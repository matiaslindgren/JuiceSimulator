#ifndef WORLD_HPP
#define WORLD_HPP
#include <list>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Box2D/Testbed/Framework/ParticleEmitter.h>
#include "DebugDraw.hpp"
#include "DrawableParticleSystem.hpp"

enum ItemTypes
{
  k_Cup = 0,
  k_Counter
};

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

    void DestroyOutOfBoundsParticles(b2ParticleSystem&);

    bool PositionOutOfBounds(const b2Vec2&) const;
    bool PositionOutOfView(const b2Vec2&) const;

    void CreateDispenser(const b2ParticleGroupDef&, const b2Vec2&);
    void CreateItem(const ItemTypes& item_type, const b2Vec2& position);

    using b2World::Step;
    void Step(const float&, const int&, const int&, const int&, sf::RenderTarget&, const bool&);

    using b2World::DrawDebugData;
    void DrawDebugData();
    void set_debug_draw(DebugDraw*);

  private:
    DebugDraw* debug_draw_;
    std::list<RadialEmitter> dispensers_;
    std::list<DrawableParticleSystem> drawable_liquids_;
    const int north_edge_;
    const int east_edge_;
    const int south_edge_;
    const int west_edge_;
};

#endif // WORLD_HPP
