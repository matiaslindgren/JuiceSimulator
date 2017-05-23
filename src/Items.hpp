#ifndef ITEMS_HPP
#define ITEMS_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Polygon.hpp"
#include "DrawableDispenser.hpp"

enum ItemTypes
{
  k_Cup = 1,
  k_Surface = 1 << 1,
  k_Box = 1 << 2,
  k_Sponge = 1 << 3,
  k_Button = 1 << 4,

  k_Movable = k_Cup | k_Box | k_Sponge
};

void CreateCup(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateBox(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateSponge(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateDispenserItem(b2Body* body, const b2Vec2& position, const float& particle_radius, sf::Texture* texture);

#endif // ITEMS_HPP
