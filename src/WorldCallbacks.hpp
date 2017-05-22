#ifndef WORLDCALLBACKS_HPP
#define WORLDCALLBACKS_HPP
#include <Box2D/Box2D.h>

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

#endif // WORLDCALLBACKS.HPP
