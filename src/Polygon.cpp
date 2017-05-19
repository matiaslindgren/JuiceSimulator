#include "Polygon.hpp"
#include "Adapter.hpp"


Polygon::Polygon(sf::Vector2f* corners, b2BodyType body_type)
  : m_vertices(sf::VertexArray(sf::Quads, 4))
{
  b2Vec2 b2_vertices[vertex_count];
  for (auto i = 0; i < vertex_count; i++)
  {
    const sf::Vector2f& corner = *corners;
    m_vertices[i].position = corner;
    b2_vertices[i].Set(corner.x, -corner.y);
    corners++;
  }

  m_shape.Set(b2_vertices, vertex_count);

  m_body_def.userData = this;
  m_body_def.type = body_type;
  /* m_body_def.position = b2_vertices[0]; */

  m_fixture_def.shape = &m_shape;
  m_fixture_def.density = 10.0f;
  m_fixture_def.restitution = 0.2f;

  m_vertices[0].color = sf::Color::Red;
  m_vertices[1].color = sf::Color::Green;
  m_vertices[2].color = sf::Color::Blue;
  m_vertices[3].color = sf::Color::Black;

};

void Polygon::applyPhysics()
{
  const b2Transform& b2_transform = m_body->GetTransform();
  sf::Transformable::setPosition(convertVector(b2_transform.p));
  sf::Transformable::setRotation(radiansToAngle(b2_transform.q.GetAngle()));
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
  // states.shader can be updated here
  target.draw(m_vertices, states);
}

