#ifndef WORLD_HPP
#define WORLD_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"


class World : public b2World
{
  public:
    World(const float& x, const float& y) : b2World(b2Vec2(x, -y)) {};

    void CreateShape(const sf::Vector2f corners[], const unsigned int& vertexCount, b2BodyType body_type);

    using b2World::Step;
    void Step(const float&, const int&, const int&, const int&, sf::RenderTarget&);

    using b2World::DrawDebugData;
    void DrawDebugData();
    void SetDebugDraw(DebugDraw*);

  private:
    DebugDraw* m_debugDraw;
};

#endif // WORLD_HPP
