#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <SFML/Graphics.hpp>
#include <string>


class TextBox : public sf::Drawable, public sf::Transformable
{
  public:
    TextBox();

    void UpdateText(const std::string& new_text, const sf::Vector2f& scale);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::RectangleShape box;
    sf::Font font;
    sf::Text text;
};

#endif // TEXTBOX.HPP
