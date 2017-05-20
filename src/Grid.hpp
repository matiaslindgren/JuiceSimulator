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
    void generate_coordinate_axes(const float& x_start, const float& x_end,
                                  const float& y_start, const float& y_end);
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray grid;
    sf::VertexArray coordinate_axes;
};


#endif // GRID.HPP
