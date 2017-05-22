#include "Polygon.hpp"
#include "Adapter.hpp"



Polygon::Polygon(const b2Vec2 vertices[],
                 const unsigned int& vertex_count,
                 const sf::Color& color)
  : vertices_(sf::VertexArray(sf::TrianglesFan, vertex_count))
{
  for (auto i = 0; i < vertex_count; i++)
  {
    vertices_[i].position = ConvertVector(vertices[i]);
    vertices_[i].color = color;
  }
}

void Polygon::ApplyPhysics(const b2Transform& b2_transform)
{
  sf::Transformable::setPosition(ConvertVector(b2_transform.p));
  sf::Transformable::setRotation(RadiansToAngle(b2_transform.q.GetAngle()));
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(vertices_, states);
}

