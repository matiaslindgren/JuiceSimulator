#ifndef CLICKABLEENTITY_HPP
#define CLICKABLEENTITY_HPP
#include "GameEntity.hpp"
#include "World.hpp"
#include <iostream>

class ClickableEntity : public GameEntity
{
  public:
    explicit ClickableEntity(std::function<void()>& OnClick)
      : GameEntity(k_Clickable),
        OnClick_(OnClick)
    {
    }
    virtual void Click()
    {
      OnClick_();
    }
  private:
    std::function<void()> OnClick_;
};

#endif // CLICKABLEENTITY_HPP
