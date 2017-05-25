#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"
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
  b2BodyDef body_def;
  ground_body_ = this->CreateBody(&body_def);
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
      delete static_cast<sf::Drawable*>(fixture->GetUserData());
      fixture->SetUserData(nullptr);
      fixture = next_fixture;
    }
    body = next_body;
  }
  ground_body_ = nullptr;
}

void World::CreateItem(const ItemTypes& item_type, const b2Vec2& position, const b2Vec2& size)
{
  b2BodyDef body_def;
  b2Body* body = nullptr;
  switch (item_type)
  {
    case k_Cup:
      body_def.type = b2_dynamicBody;
      body = this->CreateBody(&body_def);
      body->SetUserData(new ItemTypes(k_Cup));
      CreateCup(body, position, size);
      break;
    case k_Surface:
      body_def.type = b2_staticBody;
      body = this->CreateBody(&body_def);
      body->SetUserData(new ItemTypes(k_Surface));
      CreateBox(body, position, size);
      break;
    case k_Box:
      body_def.type = b2_dynamicBody;
      body = this->CreateBody(&body_def);
      body->SetUserData(new ItemTypes(k_Box));
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
  particle_system->SetGravityScale(gravity_scale);
  particle_system->SetDensity(density);
  particle_system->SetRadius(particle_radius);
  drawable_liquids_.emplace_back(particle_system->GetRadius());
}

void World::CreateSponge(const b2Vec2& position, const b2Vec2& size)
{
  b2ParticleSystem* particle_system = GetParticleSystemList();
  assert(particle_system);

  b2PolygonShape shape;
  shape.SetAsBox(size.x, size.y);
  Sponge sponge;
  sponge.position.Set(position.x, -position.y);
  sponge.shape = &shape;
  particle_system->CreateParticleGroup(sponge);
}

void World::CreateDispenser(const ParticleGroupDef& liquid_definition, const b2Vec2& position, sf::Texture* texture)
{
  b2ParticleSystem* particle_system = GetParticleSystemList();
  assert(particle_system);

  auto dispenser_id = dispensers_.size();
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
  dispenser.SetVelocity(b2Vec2(0, -80));
  dispenser.SetEmitRate(100);

  auto& dispensers = this->dispensers_;
  std::function<void()> ToggleDispenserCallback =
    [&dispensers, dispenser_id]()
    {
      std::cout << "toggled " << dispenser_id << " " << &dispensers[dispenser_id] << std::endl;
      dispensers[dispenser_id].SetEmitRate((dispensers[dispenser_id].GetEmitRate() < 1) ? 60 : 0);
      std::cout << "has emit rate " << dispensers[dispenser_id].GetEmitRate() << std::endl;
    };

  b2BodyDef body_def;
  body_def.type = b2_staticBody;

  b2Body* body = this->CreateBody(&body_def);
  body->SetUserData(new ItemTypes(k_Button));

  CreateDispenserItem(body, position, particle_radius, texture);
  CreateButton(body, position, 1, ToggleDispenserCallback);
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

    const b2Transform& body_transform = body->GetTransform();

    b2Fixture* fixture = body->GetFixtureList();
    while (fixture)
    {
      b2Fixture* next_fixture = fixture->GetNext();
      /* void* user_data = fixture->GetUserData(); */
      /* sf::Drawable* polygon = static_cast<sf::Drawable*>(user_data); */
      if (!disable_sfml_graphics)
      {
        switch (fixture->GetType())
        {
          case b2Shape::e_polygon:
            {
              // From Box2D/Dynamics/b2World.cpp
              b2PolygonShape* poly = static_cast<b2PolygonShape*>(fixture->GetShape());
              auto vertexCount = poly->m_count;
              sf::VertexArray vertices(sf::TrianglesFan, vertexCount);
              for (auto i = 0; i < vertexCount; ++i)
              {
                const b2Vec2& new_position = b2Mul(body_transform, poly->m_vertices[i]);
                vertices[i].position.x = new_position.x;
                vertices[i].position.y = -new_position.y;
                vertices[i].color = sf::Color::Blue;
              }
              render_target.draw(vertices);
            }
            break;
          case b2Shape::e_chain:
            {

              sf::Drawable* dispenser = static_cast<sf::Drawable*>(fixture->GetUserData());
              render_target.draw(*dispenser);
            }
            break;
          default:
            break;
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

