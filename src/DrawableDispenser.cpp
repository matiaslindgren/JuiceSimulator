#include "DrawableDispenser.hpp"
#include "Adapter.hpp"
#include <cmath>


DrawableDispenser::DrawableDispenser(const b2Vec2 vertices[],
                                     const unsigned int& vertex_count,
                                     sf::Texture* texture)
  : vertices_(sf::Quads, vertex_count),
    texture_(texture)
{
  for (auto i = 0; i < vertex_count; i++)
  {
    vertices_[i].position = ConvertVector(vertices[i]);
  }
  vertices_[0].texCoords = sf::Vector2f(0, 0);
  vertices_[1].texCoords = sf::Vector2f(25, 0);
  vertices_[2].texCoords = sf::Vector2f(25, 25);
  vertices_[3].texCoords = sf::Vector2f(0, 25);
}

void DrawableDispenser::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.texture = texture_;
  target.draw(vertices_, states);
}

