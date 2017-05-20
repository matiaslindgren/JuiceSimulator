#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "DebugDraw.hpp"
#include "Polygon.hpp"
#include "WorldCallbacks.hpp"


World::World(const float& gravity_x,
             const float& gravity_y,
             const int& north_edge,
             const int& east_edge,
             const int& south_edge,
             const int& west_edge)
  : b2World(b2Vec2(gravity_x, -gravity_y)),
    destruction_listener_(new DestructionListener()),
    north_edge_(north_edge),
    east_edge_(east_edge),
    south_edge_(south_edge),
    west_edge_(west_edge)
{
  b2World::SetDestructionListener(destruction_listener_);
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
  delete destruction_listener_;
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

void World::Step(const float& timeStep, const int& velocityIterations, const int& positionIterations, const int& particleIterations, sf::RenderTarget& renderTarget)
{
  // Calculate a time step in the Box2D world
  b2World::Step(timeStep, velocityIterations, positionIterations, particleIterations);
  // Update all shapes in the visual SFML world accordingly
  b2Body* body = b2World::GetBodyList();

  while (body)
  {
    Polygon* polygon = static_cast<Polygon*>(body->GetUserData());
    b2Body* nextBody = body->GetNext();
    if (BodyOutOfBounds(*body))
    {
      delete polygon;
      body->SetUserData(nullptr);
      this->DestroyBody(body);
      body = nextBody;
      continue;
    }
    polygon->ApplyPhysics(body->GetTransform());
    renderTarget.draw(*polygon);
    polygon = nullptr;
    body = nextBody;
  }

}

bool World::BodyOutOfBounds(const b2Body& body) const
{
  const b2Vec2& bodyWorldCenter = body.GetWorldCenter();
  return -bodyWorldCenter.y < north_edge_ ||
         -bodyWorldCenter.y > south_edge_ ||
         bodyWorldCenter.x < west_edge_ ||
         bodyWorldCenter.x > east_edge_;
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

