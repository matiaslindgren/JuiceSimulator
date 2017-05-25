#ifndef BUTTON_HPP
#define BUTTON_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Adapter.hpp"

class Button : public sf::Drawable
{
  public:
    Button(const b2Vec2& center,
           const float& radius,
           std::function<void()>& toggle);
    std::function<void()> toggle_;
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray vertices_;
};

#endif // BUTTON_HPP
