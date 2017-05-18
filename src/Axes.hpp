#ifndef AXES_HPP
#define AXES_HPP
#include <SFML/Graphics.hpp>


class Axes : public sf::Drawable, public sf::Transformable
{
  public:
    Axes();
  private:
    virtual void draw(sf::RenderTarget&, sf::RenderStates) const;
    sf::VertexArray m_y_axis;
    sf::VertexArray m_x_axis;
};


#endif // AXES.HPP
