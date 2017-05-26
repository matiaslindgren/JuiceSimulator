#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP
#include <Box2D/Box2D.h>
#include "World.hpp"

#include <iostream>
using std::cout;
using std::endl;

enum EntityTypes
{
  k_Default = 1,
  k_Movable = 1 << 1,
  k_Clickable = 1 << 2,
};

// Represents a wrapper around all GameItem instances that should be
// considered as a single entity and will be drawn together.
// If void* b2Body::m_userData does not point to NULL, it must point to
// one GameEntity instance.
class GameEntity
{
  public:

    GameEntity() : type_(k_Default)
    {
    }

    GameEntity(const EntityTypes& type) : type_(type)
    {
    }

    virtual ~GameEntity()
    {
    }

    // Methods called by the EventHandler when the graphical element
    // represented by this entity or its subclasses is interacted with.

    virtual void Click()
    {
    }

    virtual void Press(const b2Vec2&)
    {
    }

    virtual void Release()
    {
    }

    EntityTypes type_;
};

#endif // GAMEENTITY_HPP
