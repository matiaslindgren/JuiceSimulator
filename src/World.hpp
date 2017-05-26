#ifndef WORLD_HPP
#define WORLD_HPP
#include <vector>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <Box2D/Testbed/Framework/ParticleEmitter.h>
#include "DebugDraw.hpp"
#include "DrawableParticleSystem.hpp"
#include "Items.hpp"
#include "LiquidDefinitions.hpp"


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

    void CreateDispenser(const ParticleGroupDef&, const b2Vec2&);
    void CreateDispenserButton(const unsigned int& dispenser_index,
                               const b2Vec2& position,
                               const unsigned int& emit_rate,
                               const sf::Color& color);

    /* void CreateSponge(const b2Vec2& position, const b2Vec2& size); */
    void CreateItem(const ItemTypes& item_type, const b2Vec2& position, const b2Vec2& size);

    using b2World::Step;
    void Step(const float&, const int&, const int&, const int&, sf::RenderTarget&, const bool&);

    void DrawDebugData(const unsigned int& fps);
    void set_debug_draw(DebugDraw*);

    using b2World::CreateParticleSystem;
    void CreateParticleSystem(const float& gravity_scale,
                              const float& density,
                              const float& particle_radius);

    void CreateMouseJoint(b2Body* body, const b2Vec2& target);
    void DestroyMouseJoint();

  private:
    DebugDraw* debug_draw_;
    std::vector<RadialEmitter> dispensers_;
    int enabled_dispensers_;
    std::vector<DrawableParticleSystem> drawable_liquids_;
    const int north_edge_;
    const int east_edge_;
    const int south_edge_;
    const int west_edge_;
};

#endif // WORLD_HPP
