#ifndef DRAWABLEDISPENSER_HPP
#define DRAWABLEDISPENSER_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

class DrawableDispenser : public sf::Drawable
{
  public:
    DrawableDispenser(const b2Vec2 vertices[],
                      const unsigned int& vertex_count,
                      sf::Texture* texture);
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray vertices_;
    sf::Texture* texture_;
};

#endif // DRAWABLEDISPENSER_HPP
