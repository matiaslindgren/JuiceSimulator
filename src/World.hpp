#ifndef WORLD_HPP
#define WORLD_HPP
#include <list>
#include <array>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Shape.hpp"


class World : public b2World
{
  public:
    World(const float& x, const float& y) : b2World(b2Vec2(x, y)) {};

    Polygon& CreateShape(const sf::Texture&, float, const sf::FloatRect&);

    void TimeStep(const float&, const int&, const int&, const int& = 1);

    const std::list<Polygon>& getShapeList() const
    {
      return sfml_shapes;
    };

    std::size_t getShapeCount() const
    {
      return sfml_shapes.size();
    };

  private:
    std::list<Polygon> sfml_shapes;
};

#endif // WORLD_HPP
