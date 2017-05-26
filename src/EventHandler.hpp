#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "DebugDraw.hpp"
#include "World.hpp"

class EventHandler
{
  public:
    EventHandler()
    {
    }
    void HandleEvents(sf::RenderWindow& event_window,
                      DebugDraw* debug_draw,
                      World& world);
    void HandleDebugDraw(sf::RenderWindow& event_window,
                         const sf::Event& event,
                         DebugDraw* debug_draw);
    void HandleLeftButtonPress(const b2Vec2& mouse_position,
                               World& world);
};

#endif // EVENTHANDLER_HPP
