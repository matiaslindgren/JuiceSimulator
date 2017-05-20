#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <SFML/Graphics.hpp>


class TextBox : public sf::Drawable, public sf::Transformable
{
  public:
    TextBox();

    void UpdateText(const sf::Vector2f& position, const sf::Vector2f& scale);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::RectangleShape box;
    sf::Font font;
    sf::Text text;
};

#endif // TEXTBOX.HPP
