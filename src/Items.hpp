#ifndef ITEMS_HPP
#define ITEMS_HPP
#include <Box2D/Box2D.h>

enum ItemTypes
{
  k_Cup = 0,
  k_Counter
};

void CreateCupFixture(b2Body* body, const b2Vec2& position);
void CreateCounterFixture(b2Body* body, const b2Vec2& position);

#endif // ITEMS_HPP
