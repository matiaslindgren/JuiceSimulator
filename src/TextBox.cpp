#include <sstream>
#include <iomanip>
#include "TextBox.hpp"

TextBox::TextBox()
{
  font.loadFromFile("media/fonts/UbuntuMono-R.ttf");
  text.setFont(font);
  text.setString("N/A, N/A");
  text.setColor(sf::Color::Black);
  text.setCharacterSize(18);
}


void TextBox::UpdateText(const sf::Vector2f& position, const sf::Vector2f& scale)
{
  std::stringstream ss;
  ss << position.x << ", " << std::setprecision(2) << position.y;
  text.setString(ss.str());
  text.setScale(scale);

}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(text, states);
}
