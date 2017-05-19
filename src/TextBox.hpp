#ifndef TEXTBOX_HPP
#define TEXTBOX_HPP
#include <SFML/Graphics.hpp>


class TextBox : public sf::Drawable, public sf::Transformable
{
  public:
    TextBox(const sf::Vector2f& scale);

    void updatePosition(const float& x, const float& y);
    void updateText(const float& x, const float& y);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::RectangleShape box;
    sf::Font font;
    sf::Text text;
};

#endif // TEXTBOX.HPP
