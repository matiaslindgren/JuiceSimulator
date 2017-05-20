#include "Polygon.hpp"
#include "Adapter.hpp"


Polygon::Polygon(const sf::Vector2f corners[], const unsigned int& vertex_count)
  : vertices_(sf::VertexArray(sf::TrianglesFan, vertex_count))
{
  for (auto i = 0; i < vertex_count; i++)
  {
    vertices_[i].position = corners[i];
    vertices_[i].color = sf::Color(155, 200, 180, 150);
  }
};

void Polygon::ApplyPhysics(const b2Transform& b2_transform)
{
  sf::Transformable::setPosition(ConvertVector(b2_transform.p));
  sf::Transformable::setRotation(RadiansToAngle(b2_transform.q.GetAngle()));
}

void Polygon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  states.texture = nullptr;
  target.draw(vertices_, states);
}

