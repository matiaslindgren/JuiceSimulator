#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "DebugDraw.hpp"
#include "Polygon.hpp"

#include "Adapter.hpp"


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
  switch (item_type)
  {
    case k_Cup:
      {
        b2BodyDef body_def;
        body_def.type = b2_dynamicBody;

        b2Body* body = this->CreateBody(&body_def);

        static constexpr auto cup_height = 7;
        static constexpr auto edge_thickness = 0.5f;
        static constexpr auto side_slope = 0.2f;

        b2Vec2 left_side[] = {
          b2Vec2(0, cup_height),
          b2Vec2(edge_thickness, cup_height),
          b2Vec2(edge_thickness + side_slope, 0),
          b2Vec2(side_slope, 0)
        };

        b2Vec2 right_side[4];
        for (auto i = 0; i < 4; i++)
        {
          right_side[i] = left_side[i] + b2Vec2(cup_height/2, 0);
        }
        right_side[0].x += 2*side_slope;
        right_side[1].x += 2*side_slope;

        b2Vec2 bottom[] = {
          left_side[3],
          right_side[2],
          right_side[2] - b2Vec2(0, edge_thickness),
          left_side[3] - b2Vec2(0, edge_thickness)
        };

        b2Vec2* fixture_vertices[] = {
          left_side,
          right_side,
          bottom
        };

        for (auto i = 0; i < 3; i++)
        {
          for (auto j = 0; j < 4; j++)
          {
            fixture_vertices[i][j].x += position.x;
            fixture_vertices[i][j].y += -(position.y + cup_height);
          }
        }

        for (auto i = 0; i < 3; i++)
        {
          b2PolygonShape shape;
          shape.Set(fixture_vertices[i], 4);

          b2FixtureDef fixture_def;
          fixture_def.shape = &shape;
          fixture_def.density = 1.5f;
          fixture_def.restitution = 0;
          b2Fixture* fixture = body->CreateFixture(&fixture_def);
          fixture->SetUserData(new Polygon(fixture_vertices[i], 4, sf::Color(50, 50, 250, 150)));
        }
        break;
      }
    case k_Counter:
      {
        b2BodyDef body_def;
        body_def.type = b2_staticBody;

        static constexpr auto width = 60;
        static constexpr auto height = 1;

        b2Body* body = this->CreateBody(&body_def);

        b2Vec2 vertices[] = {
          b2Vec2(0, 0),
          b2Vec2(width, 0),
          b2Vec2(width, height),
          b2Vec2(0, height)
        };

        for (auto i = 0; i < 4; i++)
        {
          vertices[i] += position;
          vertices[i].y *= -1;
        }

        {
          b2PolygonShape shape;
          shape.Set(vertices, 4);

          b2FixtureDef fixture_def;
          fixture_def.shape = &shape;
          b2Fixture* fixture = body->CreateFixture(&fixture_def);
          fixture->SetUserData(new Polygon(vertices, 4, sf::Color(150, 150, 150, 50)));
        }
        break;
      }
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

  particle_system->CreateParticleGroup(liquid_definition);

  auto particle_radius = particle_system->GetRadius();
  dispenser.SetParticleSystem(particle_system);
  dispenser.SetColor(liquid_definition.color);
  dispenser.SetParticleFlags(liquid_definition.flags);
  dispenser.SetPosition(b2Vec2(position.x, -position.y));
  dispenser.SetSize(b2Vec2(2.1*particle_radius, 5));
  dispenser.SetSpeed(0);
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
  fixture->SetUserData(new Polygon(nozzle_vertices, 4, sf::Color(0, 0, 0, 50)));

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
  // Update all shapes in the visual SFML world accordingly
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
        if (!disable_sfml_graphics && !PositionOutOfView(body_position))
        {
          render_target.draw(*polygon);
        }
      }
      fixture = next_fixture;
    }
    body = next_body;
  }


  // Dispense some liquid from all dispensers
  for (auto& dispenser : dispensers_)
  {
    dispenser.Step(time_step, nullptr, 0);
  }
  // Draw all particle systems
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
}

bool World::PositionOutOfBounds(const b2Vec2& position) const
{
  return -position.y > south_edge_ ||
         position.x < west_edge_ ||
         position.x > east_edge_;
}

bool World::PositionOutOfView(const b2Vec2& position) const
{
  return -position.y < north_edge_;
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

