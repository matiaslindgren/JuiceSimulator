#include <array>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"
#include "DebugDraw.hpp"


Polygon& World::CreateShape(sf::Vector2f* corners, b2BodyType body_type)
{
  sfml_shapes.emplace_back(corners, body_type);
  Polygon& added = sfml_shapes.back();
  b2Body* body = this->CreateBody(added.getB2BodyDefinition());
  body->CreateFixture(added.getB2FixtureDefinition());
  added.setB2Body(body);
  return added;
}

void World::TimeStep(const float& timeStep, const int& velocityIterations, const int& positionIterations, const int& particleIterations)
{
  // Calculate a time step in the Box2D world
  b2World::Step(timeStep, velocityIterations, positionIterations, particleIterations);
  // Update all shapes in the visual SFML world accordingly
  for (auto& shape : sfml_shapes)
  {
    shape.applyPhysics();
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

