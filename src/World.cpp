#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"
#include "Polygon.hpp"
#include "World.hpp"


World::World(const float& gravity_x,
             const float& gravity_y,
             const int& north_edge,
             const int& east_edge,
             const int& south_edge,
             const int& west_edge)
  : b2World(b2Vec2(gravity_x, -gravity_y)),
    north_edge_(north_edge),
    east_edge_(east_edge),
    south_edge_(south_edge),
    west_edge_(west_edge)
{
}

World::~World()
{
  b2Body* body = b2World::GetBodyList();
  while (body)
  {
    b2Body* next_body = body->GetNext();
    b2Fixture* fixture = body->GetFixtureList();
    while (fixture)
    {
      b2Fixture* next_fixture = fixture->GetNext();
      delete static_cast<Polygon*>(fixture->GetUserData());
      fixture->SetUserData(nullptr);
      fixture = next_fixture;
    }
    body = next_body;
  }
}

void World::CreateItem(const ItemTypes& item_type, const b2Vec2& position)
{
  b2BodyDef body_def;
  switch (item_type)
  {
    case k_Cup:
      body_def.type = b2_dynamicBody;
      CreateCupFixture(this->CreateBody(&body_def), position);
      break;
    case k_Counter:
      body_def.type = b2_staticBody;
      CreateCounterFixture(this->CreateBody(&body_def), position);
      break;
    default:
      break;
  }
}

void World::CreateDispenser(const b2ParticleGroupDef& liquid_definition, const b2Vec2& position)
{
  b2ParticleSystem* particle_system = GetParticleSystemList();
  if (!particle_system)
  {
    const b2ParticleSystemDef particle_system_def;
    particle_system = b2World::CreateParticleSystem(&particle_system_def);
    particle_system->SetGravityScale(2.0f);
    particle_system->SetDensity(1.0f);
    particle_system->SetRadius(0.30f);
    drawable_liquids_.emplace_back(particle_system->GetRadius());
  }

  dispensers_.emplace_back();
  RadialEmitter& dispenser = dispensers_.back();

  b2ParticleGroup* group = particle_system->CreateParticleGroup(liquid_definition);

  auto particle_radius = particle_system->GetRadius();
  dispenser.SetParticleSystem(particle_system);
  dispenser.SetGroup(group);
  dispenser.SetColor(liquid_definition.color);
  dispenser.SetParticleFlags(liquid_definition.flags);
  dispenser.SetPosition(b2Vec2(position.x, -position.y));
  dispenser.SetSize(b2Vec2(2.1*particle_radius, 5));
  dispenser.SetSpeed(0);
  dispenser.SetVelocity(b2Vec2(0, -120));
  dispenser.SetEmitRate(80);

  b2BodyDef body_def;
  body_def.type = b2_staticBody;

  b2Body* body = this->CreateBody(&body_def);

  auto height = 35*particle_radius;
  auto width = 8*particle_radius;

  b2Vec2 nozzle_vertices[] = {
    b2Vec2(0, 0),
    b2Vec2(0, height),
    b2Vec2(width, height),
    b2Vec2(width, 0),
  };

  for (auto i = 0; i < 4; i++)
  {
    nozzle_vertices[i].x += position.x - width/2;
    nozzle_vertices[i].y += -(position.y + height/3);
  }

  b2ChainShape shape;
  shape.CreateChain(nozzle_vertices, 4);

  b2FixtureDef fixture_def;
  fixture_def.shape = &shape;
  b2Fixture* fixture = body->CreateFixture(&fixture_def);
  fixture->SetUserData(new Polygon(nozzle_vertices, 4, sf::Color(0, 0, 0, 255)));

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
  b2ParticleSystem* particle_system = GetParticleSystemList();
  while (particle_system)
  {
    b2ParticleSystem* next_particle_system = particle_system->GetNext();
    for (auto& drawable_liquid : drawable_liquids_)
    {
      if (particle_system->GetRadius() == drawable_liquid.get_particle_radius())
      {
        drawable_liquid.Step(render_target, *particle_system);
        break;
      }
    }
    DestroyOutOfBoundsParticles(*particle_system);
    particle_system = next_particle_system;
  }

  // Draw all other shapes, while removing all shapes which are
  // outside the screen
  b2Body* body = b2World::GetBodyList();

  while (body)
  {
    b2Body* next_body = body->GetNext();
    const b2Vec2& body_position = body->GetWorldCenter();
    if (PositionOutOfBounds(body_position))
    {
      DestroyBody(body);
      body = next_body;
      continue;
    }

    b2Fixture* fixture = body->GetFixtureList();
    while (fixture)
    {
      b2Fixture* next_fixture = fixture->GetNext();
      void* user_data = fixture->GetUserData();
      if (user_data)
      {
        Polygon* polygon = static_cast<Polygon*>(user_data);
        polygon->ApplyPhysics(body->GetTransform());
        if (!disable_sfml_graphics)
        {
          render_target.draw(*polygon);
        }
      }
      fixture = next_fixture;
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

void World::DrawDebugData()
{
  this->debug_draw_->DrawMouseCoordinates();
  this->debug_draw_->DrawDebugLines();
  b2World::DrawDebugData();
}

void World::DestroyOutOfBoundsParticles(b2ParticleSystem& particle_system)
{
  const b2Vec2* particle_positions = particle_system.GetPositionBuffer();
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

