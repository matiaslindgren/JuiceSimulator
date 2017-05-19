#include <sstream>
#include <iomanip>
#include "TextBox.hpp"

TextBox::TextBox()
{
  font.loadFromFile("media/fonts/UbuntuMono-R.ttf");
  text.setFont(font);
  text.setString("-,-");
  text.setColor(sf::Color::Black);
  text.setCharacterSize(35);
  text.setScale(0.008f, 0.01f);
}

void TextBox::updatePosition(const float& x, const float& y)
{
  std::stringstream ss;
  ss << x << ", " << std::setprecision(2) << y;
  text.setString(ss.str());
  text.setPosition(x, y);
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(text, states);
}
