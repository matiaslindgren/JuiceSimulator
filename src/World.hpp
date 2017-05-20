#ifndef WORLD_HPP
#define WORLD_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"


class World : public b2World
{
  public:
    World(const float& gravity_x,
          const float& gravity_y,
          const int& north_edge,
          const int& east_edge,
          const int& south_edge,
          const int& west_edge);
    ~World();

    void CreateShape(const sf::Vector2f corners[], const unsigned int& vertex_count, b2BodyType body_type);

    bool BodyOutOfBounds(const b2Body&) const;

    using b2World::Step;
    void Step(const float&, const int&, const int&, const int&, sf::RenderTarget&);

    using b2World::DrawDebugData;
    void DrawDebugData();
    void SetDebugDraw(DebugDraw*);

  private:
    DebugDraw* debug_draw_;
    b2DestructionListener* destruction_listener_;
    const int north_edge_;
    const int east_edge_;
    const int south_edge_;
    const int west_edge_;
};

#endif // WORLD_HPP
