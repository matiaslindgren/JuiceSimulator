#ifndef GRID_HPP
#define GRID_HPP
#include <SFML/Graphics.hpp>


class Grid : public sf::Drawable, public sf::Transformable
{
  public:
    Grid(const float& = -1.0f,
         const float& = -1.0f,
         const float& = 10.0f,
         const float& offset = 1.0f);
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray lines;
};


#endif // GRID.HPP
