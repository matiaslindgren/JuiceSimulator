#ifndef WORLDCALLBACKS_HPP
#define WORLDCALLBACKS_HPP
#include <Box2D/Box2D.h>
#include "Adapter.hpp"

#include <iostream>
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
    void SayGoodbye(b2Joint*)
    {
      std::cout << "NOT IMPLEMENTED: DestructionListener::SayGoodbye with parameters (b2Joint*)" << std::endl;
    }
    void SayGoodbye(b2ParticleGroup*)
    {
      std::cout << "NOT IMPLEMENTED: DestructionListener::SayGoodbye with parameters (b2ParticleGroup*)" << std::endl;
    }
    void SayGoodbye(b2ParticleSystem*, int32)
    {
      std::cout << "NOT IMPLEMENTED: DestructionListener::SayGoodbye with parameters (b2ParticleSystem*, int32)" << std::endl;
    }
};

class QueryCallback : public b2QueryCallback
{
  public:
    QueryCallback(const b2Vec2& point, const int& match_flags)
      : point_(point),
        match_flags_(match_flags),
        waldo_(nullptr)
    {
    }
    virtual bool ReportFixture(b2Fixture* fixture);
    b2Fixture* waldo_;
    const b2Vec2& point_;
  private:
    bool IsWaldoType(const int* flags) const;
    const int match_flags_;
};
#endif // WORLDCALLBACKS.HPP
