#include <sstream>
#include <iomanip>
#include "TextBox.hpp"

TextBox::TextBox(const sf::Vector2f& scale)
{
  font.loadFromFile("media/fonts/UbuntuMono-R.ttf");
  text.setFont(font);
  text.setString("-,-");
  text.setColor(sf::Color::Black);
  text.setCharacterSize(22);
  text.setScale(scale.x, scale.y);
}


void TextBox::updateText(const float& x, const float& y)
{
  std::stringstream ss;
  ss << x << ", " << std::setprecision(2) << y;
  text.setString(ss.str());
}

void TextBox::updatePosition(const float& x, const float& y)
{
  this->updateText(x, y);
  text.setPosition(x, y);
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(text, states);
}
