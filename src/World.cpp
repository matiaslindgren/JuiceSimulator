#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "DebugDraw.hpp"
#include "Polygon.hpp"
#include "WorldCallbacks.hpp"


World::World(const float& gravityX,
             const float& gravityY,
             const int& north_edge,
             const int& east_edge,
             const int& south_edge,
             const int& west_edge)
  : b2World(b2Vec2(gravityX, -gravityY)),
    m_destructionListener(new DestructionListener()),
    m_north_edge(north_edge),
    m_east_edge(east_edge),
    m_south_edge(south_edge),
    m_west_edge(west_edge)
{
  b2World::SetDestructionListener(m_destructionListener);
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
  delete m_destructionListener;
}

void World::CreateShape(const sf::Vector2f corners[], const unsigned int& vertexCount, b2BodyType body_type)
{
  b2Vec2 b2_vertices[vertexCount];
  for (auto i = 0; i < vertexCount; i++)
  {
    b2_vertices[i].Set(corners[i].x, -corners[i].y);
  }

  b2PolygonShape shape;
  shape.Set(b2_vertices, vertexCount);

  b2BodyDef body_def;
  body_def.type = body_type;

  b2FixtureDef fixture_def;
  fixture_def.shape = &shape;
  fixture_def.density = 10.0f;
  fixture_def.restitution = 0.1f;

  b2Body* body = this->CreateBody(&body_def);
  body->CreateFixture(&fixture_def);
  body->SetUserData(new Polygon(corners, vertexCount));
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
    polygon->applyPhysics(body->GetTransform());
    renderTarget.draw(*polygon);
    polygon = nullptr;
    body = nextBody;
  }

}

bool World::BodyOutOfBounds(const b2Body& body) const
{
  const b2Vec2& bodyWorldCenter = body.GetWorldCenter();
  return -bodyWorldCenter.y < m_north_edge ||
         -bodyWorldCenter.y > m_south_edge ||
         bodyWorldCenter.x < m_west_edge ||
         bodyWorldCenter.x > m_east_edge;
}

void World::SetDebugDraw(DebugDraw* debugDraw)
{
  this->m_debugDraw = debugDraw;
  b2World::SetDebugDraw(debugDraw);
  DebugLines& debuglines = this->m_debugDraw->getDebugLines();
  debuglines.generate_grid(m_north_edge, m_west_edge,
                     m_south_edge, m_east_edge, 1.0f);
  debuglines.generate_coordinate_axes(m_west_edge, m_east_edge,
                                m_north_edge, m_south_edge);
}

void World::DrawDebugData()
{
  this->m_debugDraw->DrawMouseCoordinates();
  this->m_debugDraw->DrawDebugLines();
  b2World::DrawDebugData();
}

