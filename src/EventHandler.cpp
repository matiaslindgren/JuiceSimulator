#include "Adapter.hpp"
#include "EventHandler.hpp"
#include "WorldCallbacks.hpp"
#include "Items.hpp"
#include "GameEntity.hpp"


void EventHandler::HandleDebugDraw(sf::RenderWindow& event_window,
                                   const sf::Event& event,
                                   DebugDraw* debug_draw)
{
  if (event.type == sf::Event::MouseWheelScrolled &&
      event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
  {
    sf::View view = event_window.getView();
    view.zoom(1.0f - event.mouseWheelScroll.delta/10.0f);
    event_window.setView(view);
  }
  else if (event.type == sf::Event::MouseMoved)
  {
    assert(debug_draw);
    const sf::Vector2f& mouse_position = event_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
    debug_draw->set_mouse_coordinates(mouse_position);
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::View view = event_window.getView();
    const float& offset = view.getSize().x/10.0f;
    switch(event.key.code)
    {
      case sf::Keyboard::Up:
        view.move(0.0f, -offset);
        break;
      case sf::Keyboard::Left:
        view.move(-offset, 0.0f);
        break;
      case sf::Keyboard::Down:
        view.move(0.0f, offset);
        break;
      case sf::Keyboard::Right:
        view.move(offset, 0.0f);
        break;
      default:
        break;
    }
    event_window.setView(view);
  }
}

void EventHandler::HandleLeftButtonPress(const b2Vec2& mouse_position, World& world)
{
  b2AABB aabb;
  b2Vec2 delta(0.01f, 0.01f);
  aabb.lowerBound = mouse_position - delta;
  aabb.upperBound = mouse_position + delta;

  QueryCallback callback(mouse_position, k_Movable);
  world.QueryAABB(&callback, aabb);
  b2Fixture* fixture_in_aabb = callback.waldo_;

  if (fixture_in_aabb && !world.mouse_joint_)
  { // User wants to begin moving around something that is movable
    b2Body* body = fixture_in_aabb->GetBody();
    assert(body);
    if (body)
    {
      world.CreateMouseJoint(body, mouse_position);
    }
  }
  else if (world.mouse_joint_)
  { // User is moving around something that is movable
    world.mouse_joint_->SetTarget(mouse_position);
  }
  else
  { // Check for buttons
    QueryCallback callback(mouse_position, k_Clickable);
    world.QueryAABB(&callback, aabb);
    b2Fixture* fixture_in_aabb = callback.waldo_;
    if (fixture_in_aabb)
    { // User pressed a button
      b2Body* body = fixture_in_aabb->GetBody();
      if (body)
      {
        void* body_data = body->GetUserData();
        if (body_data)
        {
          GameEntity* game_entity = static_cast<GameEntity*>(body_data);
          game_entity->Click();
        }
      }
    }
  }
}

void EventHandler::HandleEvents(sf::RenderWindow& event_window,
                                DebugDraw* debug_draw,
                                World& world)
{
  sf::Event event;
  while (event_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
      event_window.close();
    }

    if (debug_draw)
    {
      HandleDebugDraw(event_window, event, debug_draw);
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      const sf::Vector2f& mouse_position = event_window.mapPixelToCoords(sf::Mouse::getPosition(event_window));
      HandleLeftButtonPress(ConvertVector(mouse_position), world);
    }
    else if (world.mouse_joint_)
    { // The player let go of something that was being moved
      world.DestroyMouseJoint();
    }
  }
}

