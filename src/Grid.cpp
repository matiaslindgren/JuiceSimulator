#include "Grid.hpp"


Grid::Grid() : grid(sf::Lines), coordinate_axes(sf::Lines, 4)
{
}

void Grid::generate_grid(const float& top, const float& left,
                         const float& bottom, const float& right,
                         const float& spacing)
{
  grid.clear();
  const sf::Color grid_color(0, 0, 0, 40);
  for (auto y = top; y < bottom + 0.5f*spacing; y += spacing)
  {
    sf::Vertex horizontal_left(sf::Vector2f(left, y), grid_color);
    sf::Vertex horizontal_right(sf::Vector2f(right, y), grid_color);
    grid.append(horizontal_left);
    grid.append(horizontal_right);
  }
  for (auto x = left; x < right + 0.5f*spacing; x += spacing)
  {
    sf::Vertex vertical_left(sf::Vector2f(x, top), grid_color);
    sf::Vertex vertical_right(sf::Vector2f(x, bottom), grid_color);
    grid.append(vertical_left);
    grid.append(vertical_right);
  }
}

void Grid::generate_coordinate_axes(const float& x_start, const float& x_end,
                                    const float& y_start, const float& y_end)
{
  const sf::Color x_axis_color(200, 0, 0, 255);
  const sf::Color y_axis_color(0, 200, 0, 255);
  coordinate_axes[0] = sf::Vertex(sf::Vector2f(x_start, 0), x_axis_color);
  coordinate_axes[1] = sf::Vertex(sf::Vector2f(x_end, 0), x_axis_color);
  coordinate_axes[2] = sf::Vertex(sf::Vector2f(0, y_start), y_axis_color);
  coordinate_axes[3] = sf::Vertex(sf::Vector2f(0, y_end), y_axis_color);
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(grid, states);
  target.draw(coordinate_axes, states);
}

