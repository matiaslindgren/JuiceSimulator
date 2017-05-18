#include <sstream>
#include <iomanip>
#include "TextBox.hpp"

TextBox::TextBox()
{
  font.loadFromFile("media/fonts/UbuntuMono-R.ttf");
  text.setFont(font);
  text.setString("-,-");
  text.setColor(sf::Color::White);
  text.setCharacterSize(20);
  text.setPosition(0, 0);

  box.setSize(sf::Vector2f(1.0f, 0.4f));
  box.setFillColor(sf::Color::Black);
  box.setPosition(0, 0);
}

void TextBox::updatePosition(const float& x, const float& y)
{
  std::stringstream ss;
  ss << x << ", " << std::setprecision(2) << y;
  text.setString(ss.str());
  text.setPosition(x, y+0.3);
  box.setPosition(x, y);
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(box, states);
  target.draw(text, states);
}
