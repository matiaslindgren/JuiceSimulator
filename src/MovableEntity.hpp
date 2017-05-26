#ifndef MOVABLEENTITY_HPP
#define MOVABLEENTITY_HPP
#include "GameEntity.hpp"

class MovableEntity : public GameEntity
{
  public:
    MovableEntity() : GameEntity(k_Movable)
    {
    }
};

#endif // MOVABLEENTITY_HPP
