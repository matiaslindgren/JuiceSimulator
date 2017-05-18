#include "Polygon.hpp"


Polygon::Polygon(const sf::Texture& texture, const sf::FloatRect& rect, b2BodyType body_type)
  : m_texture(texture),
    m_vertices(sf::VertexArray(sf::Quads, 4))
{
  std::cout << rect.left << ", " << rect.top << ", " << rect.width  << ", " << rect.height << std::endl;
  m_vertices[0].position = sf::Vector2f(rect.left, rect.top);
  m_vertices[1].position = sf::Vector2f(rect.left, rect.top - rect.height);
  m_vertices[2].position = sf::Vector2f(rect.left + rect.width, rect.top - rect.height);
  m_vertices[3].position = sf::Vector2f(rect.left + rect.width, rect.top);

  m_vertices[0].color = sf::Color::Red;
  m_vertices[1].color = sf::Color::Green;
  m_vertices[2].color = sf::Color::Blue;
  m_vertices[3].color = sf::Color::Yellow;

  b2Vec2 b2_vertices[vertex_count];
  for (std::size_t i = 0; i < vertex_count; i++)
  {
    const sf::Vector2f& position = m_vertices[i].position;
    b2_vertices[i].Set(position.x, -position.y);
  }
  m_shape.Set(b2_vertices, vertex_count);

  m_body_def.userData = this;
  m_body_def.type = body_type;
  m_body_def.position = b2_vertices[0];

};

void Polygon::applyPhysics()
{
  const b2Vec2& b2_body_pos = m_body->GetPosition();
  const float& new_x = b2_body_pos.x;
  const float& new_y = -b2_body_pos.y;
  this->setPosition(new_x, new_y);
}

std::ostream& operator<<(std::ostream& os, const Polygon& polygon)
{
  const sf::Vector2f& sfml_pos = polygon.getPosition();
  sf::Vector2f b2_pos = polygon.getB2Position();
  return os << "sfml: " << sfml_pos.x << ", " << sfml_pos.y << " | b2: " << b2_pos.x << ", " << b2_pos.y;
};


void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  states.texture = &m_texture;
  // states.shader can be updated here
  target.draw(m_vertices, states);
}

