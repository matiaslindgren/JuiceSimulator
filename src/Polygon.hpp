#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


#include <iostream>
class Polygon : public sf::Drawable, public sf::Transformable
{
  public:
    Polygon(const sf::Vector2f corners[], const unsigned int& vertex_count);
    void ApplyPhysics(const b2Transform& b2_transform);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray vertices_;
};


#endif // POLYGON.HPP
