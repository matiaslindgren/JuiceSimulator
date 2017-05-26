#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"
#include "World.hpp"
#include "MovableEntity.hpp"
#include "ClickableEntity.hpp"
#include "GameItem.hpp"
#include "MouseJointState.hpp"


World::World(const float& gravity_x,
             const float& gravity_y,
             const int& north_edge,
             const int& east_edge,
             const int& south_edge,
             const int& west_edge)
  : b2World(b2Vec2(gravity_x, -gravity_y)),
    ground_body_(nullptr),
    mouse_joint_(nullptr),
    north_edge_(north_edge),
    east_edge_(east_edge),
    south_edge_(south_edge),
    west_edge_(west_edge)
{
  b2BodyDef body_def;
  ground_body_ = this->CreateBody(&body_def);
  ground_body_->SetUserData(nullptr);
}

World::~World()
{

  for (b2Body* body = b2World::GetBodyList();
       body;
       body = body->GetNext())
  {
    for (b2Fixture* fixture = body->GetFixtureList();
         fixture;
         fixture = fixture->GetNext())
    {
      void* game_item = fixture->GetUserData();
      if (game_item)
      {
        delete static_cast<GameItem*>(game_item);
        fixture->SetUserData(nullptr);
      }
    }
    void* game_entity = body->GetUserData();
    if (game_entity)
    {
      delete static_cast<GameEntity*>(game_entity);
      body->SetUserData(nullptr);
    }
  }

}

void World::CreateMouseJoint(b2Body* body, const b2Vec2& target)
{
  b2MouseJointDef mouse_joint_def;
  mouse_joint_def.bodyA = ground_body_;
  mouse_joint_def.bodyB = body;
  mouse_joint_def.target = target;
  mouse_joint_def.maxForce = 1000.0f * body->GetMass();
  mouse_joint_ = static_cast<b2MouseJoint*>(CreateJoint(&mouse_joint_def));
  assert(mouse_joint_);
  mouse_joint_->SetUserData(new MouseJointState());
  body->SetAwake(true);
}

void World::DestroyMouseJoint()
{
  if (mouse_joint_)
  {
    void* mouse_state = mouse_joint_->GetUserData();
    assert(mouse_state);
    if (mouse_state)
    {
      delete static_cast<MouseJointState*>(mouse_state);
      mouse_joint_->SetUserData(nullptr);
    }
    b2World::DestroyJoint(mouse_joint_);
  }
  mouse_joint_ = nullptr;
}

void World::CreateItem(const ItemTypes& item_type, const b2Vec2& position, const b2Vec2& size)
{
  b2BodyDef body_def;
  b2Body* body;
  switch (item_type)
  {
    case k_Cup:
      body_def.type = b2_dynamicBody;
      body = this->CreateBody(&body_def);
      body->SetUserData(new MovableEntity());
      assert(body);
      CreateCup(body, position, size);
      break;
    case k_Surface:
      body_def.type = b2_staticBody;
      body = this->CreateBody(&body_def);
      assert(body);
      body->SetUserData(new GameEntity());
      CreateSurface(body, position, size);
      break;
    case k_Box:
      body_def.type = b2_dynamicBody;
      body = this->CreateBody(&body_def);
      assert(body);
      body->SetUserData(new MovableEntity());
      CreateBox(body, position, size);
      break;
    default:
      break;
  }
}

void World::CreateParticleSystem(const float& gravity_scale,
                                 const float& density,
                                 const float& particle_radius)
{
  const b2ParticleSystemDef particle_system_def;
  b2ParticleSystem* particle_system = b2World::CreateParticleSystem(&particle_system_def);
  assert(particle_system);
  particle_system->SetGravityScale(gravity_scale);
  particle_system->SetDensity(density);
  particle_system->SetRadius(particle_radius);
}

/* void World::CreateSponge(const b2Vec2& position, const b2Vec2& size) */
/* { */
/*   b2ParticleSystem* particle_system = GetParticleSystemList(); */
/*   assert(particle_system); */

/*   b2PolygonShape shape; */
/*   shape.SetAsBox(size.x, size.y); */
/*   Sponge sponge; */
/*   sponge.position.Set(position.x, -position.y); */
/*   sponge.shape = &shape; */
/*   particle_system->CreateParticleGroup(sponge); */
/* } */

void World::CreateDispenser(const ParticleGroupDef& liquid_definition, const b2Vec2& position)
{
  b2ParticleSystem* particle_system = b2World::GetParticleSystemList();
  assert(particle_system);

  dispensers_.emplace_back();
  RadialEmitter& dispenser = dispensers_.back();

  auto particle_radius = particle_system->GetRadius();
  dispenser.SetParticleSystem(particle_system);
  dispenser.SetColor(liquid_definition.color);
  dispenser.SetParticleFlags(liquid_definition.flags);
  dispenser.SetPosition(b2Vec2(position.x, -position.y));
  dispenser.SetSize(b2Vec2(2.1*particle_radius, 5));
  dispenser.SetSpeed(0);
  dispenser.SetVelocity(b2Vec2(0, -80));
  dispenser.SetEmitRate(0);

  b2BodyDef body_def;
  body_def.type = b2_staticBody;

  b2Body* body = this->CreateBody(&body_def);
  body->SetUserData(new GameEntity());

  CreateDispenserItem(body, position, particle_radius, liquid_definition.color);
  drawable_particle_system_.GenerateVertices(particle_radius);
}

void World::CreateDispenserButton(const unsigned int& dispenser_index, const b2Vec2& position, const unsigned int& emit_rate, const sf::Color& color)
{
  // Construct a closure for toggling on/off the dispenser
  auto& dispensers = this->dispensers_;
  std::function<void()> toggle_dispenser =
    [&dispensers, dispenser_index, emit_rate]()
    {
      dispensers[dispenser_index].SetEmitRate(emit_rate);
    };

  b2BodyDef body_def;
  body_def.type = b2_staticBody;

  b2Body* body = this->CreateBody(&body_def);
  body->SetUserData(new ClickableEntity(toggle_dispenser));
  CreateButton(body, position, 0.8, color);
}

void World::Step(const float&      time_step,
                 const int&        velocity_iterations,
                 const int&        position_iterations,
                 const int&        particle_iterations,
                 sf::RenderTarget& render_target,
                 const bool&       disable_sfml_graphics)
{
  // Calculate a time step in the Box2D world
  b2World::Step(time_step, velocity_iterations,
                position_iterations, particle_iterations);

  // Dispense some liquid from all dispensers
  for (auto& dispenser : dispensers_)
  {
    dispenser.Step(time_step, nullptr, 0);
  }

  // Draw all particle systems, while removing all particles which are
  // outside the screen
  for (b2ParticleSystem* particle_system = GetParticleSystemList();
       particle_system;
       particle_system = particle_system->GetNext())
  {
    drawable_particle_system_.Step(render_target, *particle_system);
    DestroyOutOfBoundsParticles(*particle_system);
  }

  // Draw all other shapes, while removing all shapes which are
  // outside the screen
  b2Body* body = b2World::GetBodyList();
  while (body)
  {
    b2Body* next_body = body->GetNext();
    const b2Vec2& body_position = body->GetWorldCenter();
    if (PositionOutOfBounds(body_position) &&
        !(mouse_joint_ &&
          (mouse_joint_->GetBodyA() == body ||
           mouse_joint_->GetBodyB() == body)))
      { // This body is outside the screen and nobody is holding on to it
        // with their mouse, so we can destroy the body.
        if (void* body_data = body->GetUserData())
          delete static_cast<GameEntity*>(body_data);
        body->SetUserData(nullptr);
        b2World::DestroyBody(body);
        body = next_body;
        continue;
      }

    const b2Transform& body_transform = body->GetTransform();

    for (b2Fixture* fixture = body->GetFixtureList();
         fixture;
         fixture = fixture->GetNext())
    {
      if (!disable_sfml_graphics)
      {
        void* fixture_data = fixture->GetUserData();
        assert(fixture_data);
        if (fixture_data)
        {
          GameItem* game_item = static_cast<GameItem*>(fixture_data);
          game_item->ApplyTransform(body_transform);
          game_item->Draw(render_target);
        }
      }
    }
    body = next_body;
  }
}

bool World::PositionOutOfBounds(const b2Vec2& position) const
{
  return -position.y > south_edge_ ||
         position.x < west_edge_ ||
         position.x > east_edge_;
}

void World::set_debug_draw(DebugDraw* debug_draw)
{
  this->debug_draw_ = debug_draw;
  b2World::SetDebugDraw(debug_draw);
  DebugLines& debuglines = this->debug_draw_->get_debuglines();
  debuglines.GenerateGrid(north_edge_, west_edge_,
                          south_edge_, east_edge_, 1.0f);
  debuglines.GenerateCoordinateAxes(west_edge_, east_edge_,
                                    north_edge_, south_edge_);
}

void World::DrawDebugData(const unsigned int& fps)
{
  this->debug_draw_->DrawMouseCoordinates();
  this->debug_draw_->DrawDebugLines();
  this->debug_draw_->DrawFPS(fps);
  b2World::DrawDebugData();
}

void World::DestroyOutOfBoundsParticles(b2ParticleSystem& particle_system)
{
  const b2Vec2* particle_positions = particle_system.GetPositionBuffer();
  assert(particle_positions);
  const auto particle_count = particle_system.GetParticleCount();
  for (auto i = 0; i < particle_count; i++)
  {
    if (PositionOutOfBounds(particle_positions[i]))
    {
      auto active_flags = particle_system.GetParticleFlags(i);
      particle_system.SetParticleFlags(i, active_flags | b2_zombieParticle);
    }
  }
}

