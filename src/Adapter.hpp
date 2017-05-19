#ifndef ADAPTER_HPP
#define ADAPTER_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

sf::Color convertColor(const b2Color&);
sf::Vector2f convertVector(const b2Vec2&);
b2Vec2 convertVector(const sf::Vector2f&);

#endif // ADAPTER.HPP
