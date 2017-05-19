#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


class Polygon : public sf::Drawable, public sf::Transformable
{
  public:
    Polygon(const sf::Vector2f corners[], const unsigned int& vertexCount);
    void applyPhysics(const b2Transform& b2_transform);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    sf::VertexArray m_vertices;
};


#endif // POLYGON.HPP
