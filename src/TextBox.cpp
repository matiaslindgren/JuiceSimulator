#include "TextBox.hpp"

TextBox::TextBox()
{
  font.loadFromFile("media/fonts/UbuntuMono-R.ttf");
  text.setFont(font);
  text.setString("N/A");
  text.setColor(sf::Color::Black);
  text.setCharacterSize(18);
}


void TextBox::UpdateText(const std::string& new_text, const sf::Vector2f& scale)
{
  text.setString(new_text);
  text.setScale(scale);
}

void TextBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();
  target.draw(text, states);
}
