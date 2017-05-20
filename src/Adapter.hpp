#ifndef ADAPTER_HPP
#define ADAPTER_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

sf::Color ConvertColor(const b2Color&);
sf::Vector2f ConvertVector(const b2Vec2&);
b2Vec2 ConvertVector(const sf::Vector2f&);
float RadiansToAngle(const float32&);

#endif // ADAPTER.HPP
