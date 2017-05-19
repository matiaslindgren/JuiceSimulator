#include <Box2D/Common/b2Settings.h> // b2_pi
#include "Adapter.hpp"

constexpr float DEGREES_PER_RADIAN = 180.0f/b2_pi;

sf::Color convertColor(const b2Color& b2_color)
{
  return sf::Color(b2_color.r*255, b2_color.g*255, b2_color.b*255);
}

sf::Vector2f convertVector(const b2Vec2& b2_vector)
{
  return sf::Vector2f(b2_vector.x, -b2_vector.y);
}

b2Vec2 convertVector(const sf::Vector2f& sfml_vector)
{
  return b2Vec2(sfml_vector.x, -sfml_vector.y);
}

float radiansToAngle(const float32& radians)
{
  return -radians*DEGREES_PER_RADIAN;
}
