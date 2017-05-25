#include "WorldCallbacks.hpp"
#include "Items.hpp"

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
      delete static_cast<ItemTypes*>(actor_flag);
      body->SetUserData(nullptr);
    }
  }
}

// This is getting a bit too hacky
void DestructionListener::SayGoodbye(b2Joint* joint)
{
  void* joint_user_data = joint->GetUserData();
  if (joint_user_data)
  {
    bool is_mouse_joint = *static_cast<bool*>(joint_user_data);
    if (is_mouse_joint)
    { // If the joint was the mouse joint used to move around an item,
      // it has a pointer in the EventHandler instance
      // Set it to null so that the EventHandler wont create a segfault by
      // trying to destroy it again
      (static_cast<EventHandler*>(joint_user_data))->mouse_joint_ = nullptr;
    }
  }
}

bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
  b2Body* body = fixture->GetBody();
  if (!body)
    return true;
  void* actor_flag = body->GetUserData();
  if (actor_flag &&
      IsWaldoType(static_cast<int*>(actor_flag)) &&
      fixture->TestPoint(point_))
  {
    waldo_ = fixture;
    return false;
  }
  return true;
}

bool QueryCallback::IsWaldoType(const int* flag) const
{
  return (match_flags_ & *flag) == *flag;
}

