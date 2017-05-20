#ifndef WORLDCALLBACKS_HPP
#define WORLDCALLBACKS_HPP
#include <Box2D/Box2D.h>

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
    }
    void SayGoodbye(b2ParticleGroup*)
    {
    }
    void SayGoodbye(b2ParticleSystem*, int32)
    {
    }
};


#endif // WORLDCALLBACKS.HPP
