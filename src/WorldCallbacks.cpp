#include "WorldCallbacks.hpp"
#include "Polygon.hpp"

void DestructionListener::SayGoodbye(b2Fixture* fixture)
{
  void* user_data = fixture->GetUserData();
  if (user_data)
  {
    delete static_cast<Polygon*>(user_data);
    fixture->SetUserData(nullptr);
  }
}

