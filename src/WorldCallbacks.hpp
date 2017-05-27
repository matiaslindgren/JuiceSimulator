#ifndef WORLDCALLBACKS_HPP
#define WORLDCALLBACKS_HPP
#include <Box2D/Box2D.h>
#include "Adapter.hpp"
#include "EventHandler.hpp"
#include "GameEntity.hpp"


// Destruction listener for deleting the Polygon object assigned
// to every userData pointer in bodies
class DestructionListener : public b2DestructionListener
{
  public:
    DestructionListener()
    {
    }
    ~DestructionListener()
    {
    }
    void SayGoodbye(b2Fixture*);
    void SayGoodbye(b2Joint* joint);

    void SayGoodbye(b2ParticleGroup*)
    {
    }
    void SayGoodbye(b2ParticleSystem*, int32)
    {
    }
};

class QueryCallback : public b2QueryCallback
{
  public:
    // Construct a callback object where Box2D can store
    // a fixture pointer to the fixture that is at point and
    // its body matches the given entity type
    QueryCallback(const b2Vec2& point, const EntityTypes& match_flags)
      : waldo_(nullptr),
        point_(point),
        match_flags_(match_flags)
    {
    }

    // Predicate deciding whether the fixture given as
    // parameter is Waldo or not.
    virtual bool ReportFixture(b2Fixture* fixture);

    // After the query, this pointer will point to the fixture which
    // matched the query, else it is NULL.
    b2Fixture* waldo_;

  private:
    // Predicate for comparing if the entity flag given as parameter
    // matches the flags of objects which might be Waldo.
    bool IsWaldoType(const EntityTypes& flag) const;
    const b2Vec2& point_;
    const EntityTypes match_flags_;
};
#endif // WORLDCALLBACKS.HPP
