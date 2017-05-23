#ifndef ITEMS_HPP
#define ITEMS_HPP
#include <Box2D/Box2D.h>

enum ItemTypes
{
  k_Cup = 0,
  k_Surface,
  k_Box,
  k_Sponge
};

void CreateCup(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateBox(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateSponge(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateDispenserItem(b2Body* body, const b2Vec2& position, const float& particle_radius);

#endif // ITEMS_HPP
