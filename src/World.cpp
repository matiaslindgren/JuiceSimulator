#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "DebugDraw.hpp"
#include "Polygon.hpp"


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
    if (-body->GetWorldCenter().y > 25 ||
        body->GetWorldCenter().x > 25 ||
        body->GetWorldCenter().x < -5)
    {
      delete polygon;
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

void World::SetDebugDraw(DebugDraw* debugDraw)
{
  this->m_debugDraw = debugDraw;
  b2World::SetDebugDraw(debugDraw);
}

void World::DrawDebugData()
{
  this->m_debugDraw->DrawMouseCoordinates();
  this->m_debugDraw->DrawGrid();
  b2World::DrawDebugData();
}

