#include "Axes.hpp"
#include "Main.hpp"


Axes::Axes() : m_y_axis(sf::Lines, 2), m_x_axis(sf::Lines, 2)
{
  m_y_axis[0].position = sf::Vector2f(0, -1);
  m_y_axis[1].position = sf::Vector2f(0, 10);
  m_x_axis[0].position = sf::Vector2f(-1, 0);
  m_x_axis[1].position = sf::Vector2f(10, 0);
  m_y_axis[0].color = sf::Color::Black;
  m_y_axis[1].color = sf::Color::Black;
  m_x_axis[0].color = sf::Color::Black;
  m_x_axis[1].color = sf::Color::Black;
}

void Axes::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform() * SCALE_WORLD;
  target.draw(m_y_axis, states);
  target.draw(m_x_axis, states);
}

