#ifndef WORLD_HPP
#define WORLD_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"


class World : public b2World
{
  public:
    World(const float& x, const float& y) : b2World(b2Vec2(x, -y)) {};

    Polygon& CreateShape(sf::Vector2f* corners, b2BodyType body_type);

    void TimeStep(const float&, const int&, const int&, const int& = 1);

    const std::list<Polygon>& getShapeList() const
    {
      return sfml_shapes;
    };

    std::size_t getShapeCount() const
    {
      return sfml_shapes.size();
    };

    using b2World::DrawDebugData;
    void DrawDebugData();
    void SetDebugDraw(DebugDraw*);

  private:
    std::list<Polygon> sfml_shapes;
    DebugDraw* m_debugDraw;
};

#endif // WORLD_HPP
