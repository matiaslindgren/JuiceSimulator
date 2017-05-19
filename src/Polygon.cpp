#include "Polygon.hpp"
#include "Adapter.hpp"


Polygon::Polygon(const sf::Vector2f corners[], const unsigned int& vertexCount)
  : m_vertices(sf::VertexArray(sf::TrianglesFan, vertexCount))
{
  for (auto i = 0; i < vertexCount; i++)
  {
    m_vertices[i].position = corners[i];
    m_vertices[i].color = sf::Color(155, 200, 180, 150);
  }
};

void Polygon::applyPhysics(const b2Transform& b2_transform)
{
  sf::Transformable::setPosition(convertVector(b2_transform.p));
  sf::Transformable::setRotation(radiansToAngle(b2_transform.q.GetAngle()));
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  states.texture = nullptr;
  target.draw(m_vertices, states);
}

