#include "WorldCallbacks.hpp"
#include "Polygon.hpp"

void DestructionListener::SayGoodbye(b2Fixture* fixture)
{
  b2Body* body = fixture->GetBody();
  if (body)
  {
    delete static_cast<Polygon*>(body->GetUserData());
    body->SetUserData(nullptr);
  }
}
