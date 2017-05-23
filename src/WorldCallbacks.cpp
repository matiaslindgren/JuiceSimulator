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



bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
  if (fixture->GetBody()->GetType() != b2_dynamicBody)
  {
    return true;
  }
  if (fixture->TestPoint(point_))
  {
    matching_fixture_ = fixture;
  }
  return false;
}

