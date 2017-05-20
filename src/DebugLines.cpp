#include "DebugLines.hpp"


DebugLines::DebugLines() : grid_(sf::Lines), coordinate_axes_(sf::Lines, 4)
{
}

void DebugLines::GenerateGrid(const float& top, const float& left,
                         const float& bottom, const float& right,
                         const float& spacing)
{
  grid_.clear();
  const sf::Color grid_color(0, 0, 0, 40);
  for (auto y = top; y < bottom + 0.5f*spacing; y += spacing)
  {
    sf::Vertex horizontal_left(sf::Vector2f(left, y), grid_color);
    sf::Vertex horizontal_right(sf::Vector2f(right, y), grid_color);
    grid_.append(horizontal_left);
    grid_.append(horizontal_right);
  }
  for (auto x = left; x < right + 0.5f*spacing; x += spacing)
  {
    sf::Vertex vertical_left(sf::Vector2f(x, top), grid_color);
    sf::Vertex vertical_right(sf::Vector2f(x, bottom), grid_color);
    grid_.append(vertical_left);
    grid_.append(vertical_right);
  }
}

void DebugLines::GenerateCoordinateAxes(const float& x_start, const float& x_end,
                                    const float& y_start, const float& y_end)
{
  const sf::Color x_axis_color(200, 0, 0, 255);
  const sf::Color y_axis_color(0, 200, 0, 255);
  coordinate_axes_[0] = sf::Vertex(sf::Vector2f(x_start, 0), x_axis_color);
  coordinate_axes_[1] = sf::Vertex(sf::Vector2f(x_end, 0), x_axis_color);
  coordinate_axes_[2] = sf::Vertex(sf::Vector2f(0, y_start), y_axis_color);
  coordinate_axes_[3] = sf::Vertex(sf::Vector2f(0, y_end), y_axis_color);
}

void DebugLines::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(grid_, states);
  target.draw(coordinate_axes_, states);
}

