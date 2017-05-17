#include <list>
#include <array>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "World.hpp"


Polygon& World::CreateShape(const sf::Texture& texture, float density, const sf::FloatRect& rect)
{
  sfml_shapes.emplace_back(texture, rect);
  Polygon& added = sfml_shapes.front();
  b2Body* body = this->CreateBody(added.getB2BodyDefinition());
  body->CreateFixture(added.getB2Polygon(), density);
  added.setB2Body(body);
  return added;
}

void World::TimeStep(const float& timeStep, const int& velocityIterations, const int& positionIterations, const int& particleIterations)
{
  // Calculate a time step in the Box2D world
  b2World::Step(timeStep, velocityIterations, positionIterations, particleIterations);
  // Update all shapes in the visual SFML world accordingly
  for (auto& shape : sfml_shapes)
    shape.applyPhysics();

}

