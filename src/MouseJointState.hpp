#ifndef MOUSEJOINTSTATE_HPP
#define MOUSEJOINTSTATE_HPP

// Stores the global state of the mouse joint used for moving around objects.
// There is only a single active mouse joint in the world.
// If void* b2Joint::m_userData does not point to NULL, it must point to
// one MouseJointState instance.
class MouseJointState
{
  public:
    MouseJointState() : enabled_(true)
    {
    }
    bool enabled_;
};

#endif // MOUSEJOINTSTATE_HPP
