#include "WorldCallbacks.hpp"
#include "Items.hpp"
#include "GameItem.hpp"
#include "MouseJointState.hpp"
#include "GameEntity.hpp"

void DestructionListener::SayGoodbye(b2Fixture* fixture)
{
  assert(fixture);
  void* game_item = fixture->GetUserData();
  if (game_item)
  {
    delete static_cast<GameItem*>(game_item);
    fixture->SetUserData(nullptr);
  }
}

void DestructionListener::SayGoodbye(b2Joint* joint)
{
  void* mouse_joint_state = joint->GetUserData();
  if (mouse_joint_state)
  {
    delete static_cast<MouseJointState*>(mouse_joint_state);
    joint->SetUserData(nullptr);
  }
}

bool QueryCallback::ReportFixture(b2Fixture* fixture)
{
  b2Body* body = fixture->GetBody();
  // A fixture should always be part of a body which in
  // turn contains a GameEntity object which can
  // be interacted with.
  assert(body);
  void* body_data = body->GetUserData();
  assert(body_data);
  GameEntity* game_entity = static_cast<GameEntity*>(body_data);
  if (IsWaldoType(game_entity->type_) && fixture->TestPoint(point_))
  {
    waldo_ = fixture;
    return false;
  }
  return true;
}

bool QueryCallback::IsWaldoType(const EntityTypes& flag) const
{
  return (match_flags_ & flag) == flag;
}

