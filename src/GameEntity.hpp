#ifndef GAMEENTITY_HPP
#define GAMEENTITY_HPP

// Represents a wrapper around all GameItem instances that should be
// considered as a single entity and will be drawn together.
// If void* b2Body::m_userData does not point to NULL, it must point to
// one GameEntity instance.
class GameEntity
{
  public:
    GameEntity();
};

#endif // GAMEENTITY_HPP
