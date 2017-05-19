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
    m_vertices[i].color = sf::Color(155, 200, 180, 150);
    b2_vertices[i].Set(corner.x, -corner.y);
    corners++;
  }

  m_shape.Set(b2_vertices, vertex_count);

  m_body_def.userData = this;
  m_body_def.type = body_type;

  m_fixture_def.shape = &m_shape;
  m_fixture_def.density = 0.1f;
  m_fixture_def.restitution = 0.4f;
  m_fixture_def.friction = 0.7f;

};

void Polygon::applyPhysics()
{
  const b2Transform& b2_transform = m_body->GetTransform();
  sf::Transformable::setPosition(convertVector(b2_transform.p));
  sf::Transformable::setRotation(radiansToAngle(b2_transform.q.GetAngle()));
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  states.texture = nullptr;
  target.draw(m_vertices, states);
}

