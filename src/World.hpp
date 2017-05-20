#ifndef WORLD_HPP
#define WORLD_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"


class World : public b2World
{
  public:
    World(const float& gravityX,
          const float& gravityY,
          const int& north_edge,
          const int& east_edge,
          const int& south_edge,
          const int& west_edge);
    ~World();

    void CreateShape(const sf::Vector2f corners[], const unsigned int& vertexCount, b2BodyType body_type);

    bool BodyOutOfBounds(const b2Body&) const;

    using b2World::Step;
    void Step(const float&, const int&, const int&, const int&, sf::RenderTarget&);

    using b2World::DrawDebugData;
    void DrawDebugData();
    void SetDebugDraw(DebugDraw*);

  private:
    DebugDraw* m_debugDraw;
    b2DestructionListener* m_destructionListener;
    const int m_north_edge;
    const int m_east_edge;
    const int m_south_edge;
    const int m_west_edge;
};

#endif // WORLD_HPP
