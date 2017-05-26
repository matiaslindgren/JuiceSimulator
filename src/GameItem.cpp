#include "Adapter.hpp"
#include "GameItem.hpp"
#include <iostream>


GameItem::GameItem(const b2Vec2 b2_vertices[],
                   const unsigned int& vertex_count,
                   const sf::PrimitiveType& sfml_vertex_primitive,
                   const sf::Color& color,
                   const sf::RenderStates& render_states)
  : draw_vertices_(sfml_vertex_primitive, vertex_count),
    render_states_(render_states)
{
  assert(shape_vertices_.max_size() >= vertex_count);
  shape_vertices_.resize(vertex_count);
  for (unsigned int i = 0; i < vertex_count; i++)
  {
    shape_vertices_[i] = b2_vertices[i];
    draw_vertices_[i].color = color;
  }
}


void GameItem::ApplyTransform(const b2Transform& b2_transform)
{
  for (unsigned int i = 0; i < shape_vertices_.size(); i++)
  {
    const b2Vec2& new_position = b2Mul(b2_transform, shape_vertices_[i]);
    draw_vertices_[i].position = ConvertVector(new_position);
  }
}

void GameItem::Draw(sf::RenderTarget& target)
{
  target.draw(draw_vertices_, render_states_);
}

