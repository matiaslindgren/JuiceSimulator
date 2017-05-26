#ifndef ITEMS_HPP
#define ITEMS_HPP
#include <Box2D/Box2D.h>

enum ItemTypes
{
  k_Cup = 1,
  k_Surface = 1 << 1,
  k_Box = 1 << 2,
  k_Sponge = 1 << 3,
  k_Button = 1 << 4,
};

void CreateCup(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateBox(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateSurface(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateSponge(b2Body* body, const b2Vec2& position, const b2Vec2& size);
void CreateDispenserItem(b2Body* body, const b2Vec2& position, const float& particle_radius, const b2ParticleColor& color);
void CreateButton(b2Body* body, const b2Vec2& position, const float& radius, const sf::Color& color);

#endif // ITEMS_HPP
