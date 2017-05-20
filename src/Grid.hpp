#ifndef GRID_HPP
#define GRID_HPP
#include <SFML/Graphics.hpp>


class Grid : public sf::Drawable
{
  public:
    Grid();
    void generate_grid(const float& top, const float& left,
                       const float& bottom, const float& right,
                       const float& spacing);
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray lines;
};


#endif // GRID.HPP
