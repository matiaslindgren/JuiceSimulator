#include "Items.hpp"
#include "Polygon.hpp"


void CreateCup(b2Body* body, const b2Vec2& position, const b2Vec2& size)
{
  const float& width = size.x;
  const float& height = size.y;
  static constexpr auto edge_thickness = 0.5f;
  static constexpr auto side_slope = 0.2f;

  b2Vec2 left_side[] = {
    b2Vec2(0, height),
    b2Vec2(edge_thickness, height),
    b2Vec2(edge_thickness + side_slope, 0),
    b2Vec2(side_slope, 0)
  };

  b2Vec2 right_side[4];
  for (auto i = 0; i < 4; i++)
  {
    right_side[i] = left_side[i] + b2Vec2(width, 0);
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
      fixture_vertices[i][j].y += -(position.y + height);
    }
  }

  for (auto i = 0; i < 3; i++)
  {
    b2PolygonShape shape;
    shape.Set(fixture_vertices[i], 4);

    b2FixtureDef fixture_def;
    fixture_def.shape = &shape;
    // Cups are light and bouncy
    fixture_def.density = 0.4f;
    fixture_def.restitution = 0.5f;
    b2Fixture* fixture = body->CreateFixture(&fixture_def);
    fixture->SetUserData(new Polygon(fixture_vertices[i], 4, sf::Color(50, 50, 250, 150)));
  }
}


void CreateBox(b2Body* body, const b2Vec2& position, const b2Vec2& size)
{
  const float& width = size.x;
  const float& height = size.y;

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
}
