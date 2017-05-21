#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "DebugDraw.hpp"
#include "Polygon.hpp"


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
    b2Body* nextBody = body->GetNext();
    delete static_cast<Polygon*>(body->GetUserData());
    body->SetUserData(nullptr);
    body = nextBody;
  }
}

void World::CreateShape(const sf::Vector2f corners[], const unsigned int& vertex_count, b2BodyType body_type)
{
  b2Vec2 b2_vertices[vertex_count];
  for (auto i = 0; i < vertex_count; i++)
  {
    b2_vertices[i].Set(corners[i].x, -corners[i].y);
  }

  b2PolygonShape shape;
  shape.Set(b2_vertices, vertex_count);

  b2BodyDef body_def;
  body_def.type = body_type;

  b2FixtureDef fixture_def;
  fixture_def.shape = &shape;
  fixture_def.density = 10.0f;
  fixture_def.restitution = 0.1f;

  b2Body* body = this->CreateBody(&body_def);
  body->CreateFixture(&fixture_def);
  body->SetUserData(new Polygon(corners, vertex_count));
}

void World::CreateParticleSystem(const b2Vec2& start_pos)
{
  std::cout << "CreateParticleSystem" << std::endl;
  const b2ParticleSystemDef particle_system_def;
	particle_system_ = b2World::CreateParticleSystem(&particle_system_def);
  std::cout << "b2World::CreateParticleSystem" << std::endl;
	particle_system_->SetGravityScale(40.0f);
	particle_system_->SetDensity(0.2f);
	particle_system_->SetRadius(0.15f);
  particle_system_->SetDamping(2.0001f);

  b2ParticleGroupDef particle_group_def;
  b2PolygonShape b2_polygon;
  b2_polygon.SetAsBox(5.5f, 5.5f);
  particle_group_def.shape = &b2_polygon;
  particle_group_def.flags = b2_waterParticle;
  particle_group_def.position.Set(start_pos.x, -start_pos.y);
  particle_group_def.color.Set(10, 50, 230, 100);
  particle_group_ = particle_system_->CreateParticleGroup(particle_group_def);
  std::cout << "particle_system_->CreateParticleGroup" << std::endl;
}

void World::Step(const float&      timeStep,
                 const int&        velocityIterations,
                 const int&        positionIterations,
                 const int&        particleIterations,
                 sf::RenderTarget& renderTarget,
                 const bool&       disable_sfml_graphics)
{
  // Calculate a time step in the Box2D world
  b2World::Step(timeStep, velocityIterations, positionIterations, particleIterations);
  // Update all shapes in the visual SFML world accordingly
  b2Body* body = b2World::GetBodyList();

  while (body)
  {
    void* user_data = body->GetUserData();
    b2Body* nextBody = body->GetNext();
    const b2Vec2& body_position = body->GetWorldCenter();
    if (PositionOutOfBounds(body_position))
    {
      DestroyBody(body);
    }
    else if (user_data)
    {
      Polygon* polygon = static_cast<Polygon*>(user_data);
      polygon->ApplyPhysics(body->GetTransform());
      if (!disable_sfml_graphics && !PositionOutOfView(body_position))
      {
        renderTarget.draw(*polygon);
      }
    }
    body = nextBody;
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

void World::SetDebugDraw(DebugDraw* debug_draw)
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

