#include "WorldCallbacks.hpp"

void DestructionListener::SayGoodbye(b2Fixture* fixture)
{
  void* drawable = fixture->GetUserData();
  if (drawable)
  {
    delete static_cast<sf::Drawable*>(drawable);
    fixture->SetUserData(nullptr);
  }
  b2Body* body = fixture->GetBody();
  if (body)
  {
    void* actor_flag = body->GetUserData();
    if (actor_flag)
    {
      delete static_cast<int*>(actor_flag);
      body->SetUserData(nullptr);
    }
  }
}


bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
  void* actor_flag = fixture->GetBody()->GetUserData();
  if (actor_flag &&
      IsWaldoType(static_cast<int*>(actor_flag)) &&
      fixture->TestPoint(point_))
  {
    waldo_ = fixture;
    return false;
  }
  return true;
}

bool QueryCallback::IsWaldoType(const int* flags) const
{
  return (*flags & match_flags_) == match_flags_;
}

