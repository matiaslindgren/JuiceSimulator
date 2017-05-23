#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"
#include "World.hpp"
#include "WorldCallbacks.hpp"


class EventHandler
{
  public:
    EventHandler();
    void HandleEvents(sf::RenderWindow& event_window,
                      DebugDraw* debug_draw,
                      World& world);
    void HandleDebugDraw(sf::RenderWindow& event_window,
                         const sf::Event& event,
                         DebugDraw* debug_draw);
  private:
    b2MouseJoint* mouse_joint_;
};

#endif // EVENTHANDLER_HPP
