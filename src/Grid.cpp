#include "Grid.hpp"


Grid::Grid() : lines(sf::Lines)
{
}

void Grid::generate_grid(const float& top, const float& left,
                         const float& bottom, const float& right,
                         const float& spacing)
{
  lines.clear();
  for (auto y = top; y < bottom + 0.5f*spacing; y += spacing)
  {
    sf::Vertex horizontal_left(sf::Vector2f(left, y), sf::Color::Black);
    sf::Vertex horizontal_right(sf::Vector2f(right, y), sf::Color::Black);
    lines.append(horizontal_left);
    lines.append(horizontal_right);
  }
  for (auto x = left; x < right + 0.5f*spacing; x += spacing)
  {
    sf::Vertex vertical_left(sf::Vector2f(x, top), sf::Color::Black);
    sf::Vertex vertical_right(sf::Vector2f(x, bottom), sf::Color::Black);
    lines.append(vertical_left);
    lines.append(vertical_right);
  }
}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(lines, states);
}

