#include "WorldCallbacks.hpp"
#include "Polygon.hpp"

void DestructionListener::SayGoodbye(b2Fixture* fixture)
{
  b2Body* body = fixture->GetBody();
  if (body)
  {
    void* user_data = body->GetUserData();
    if (user_data)
    {
      delete static_cast<Polygon*>(user_data);
      body->SetUserData(nullptr);
    }
  }
}

void ContactListener::BeginContact(b2Contact* contact)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << " void BeginContact(b2Contact*)" << std::endl;
}

void ContactListener::EndContact(b2Contact* contact)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << " void EndContact(b2Contact*)" << std::endl;
}

void ContactListener::BeginContact(b2ParticleSystem* particle_system, b2ParticleBodyContact* particle_body_contact)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << " void BeginContact(b2ParticleSystem*, b2ParticleBodyContact*)" << std::endl;
}

void ContactListener::EndContact(b2Fixture* fixture, b2ParticleSystem* particle_system, int32 particle_index)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << " void EndContact(b2Fixture*, b2ParticleSystem*, int32)" << std::endl;
}

void ContactListener::BeginContact(b2ParticleSystem*, b2ParticleContact*)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << " void BeginContact(b2ParticleSystem*, b2ParticleContact*)" << std::endl;
}

void ContactListener::EndContact(b2ParticleSystem*, int32, int32)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << " void EndContact(b2ParticleSystem*, int32, int32)" << std::endl;
}
