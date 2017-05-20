#ifndef DEBUGLINES_HPP
#define DEBUGLINES_HPP
#include <SFML/Graphics.hpp>


class DebugLines : public sf::Drawable
{
  public:
    DebugLines();
    void GenerateGrid(const float& top, const float& left,
                       const float& bottom, const float& right,
                       const float& spacing);
    void GenerateCoordinateAxes(const float& x_start, const float& x_end,
                                const float& y_start, const float& y_end);
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray grid_;
    sf::VertexArray coordinate_axes_;
};


#endif // DEBUGLINES.HPP
