#include "Adapter.hpp"
#include "EventHandler.hpp"
#include "WorldCallbacks.hpp"


EventHandler::EventHandler()
  : mouse_joint_(nullptr)
{
}


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
      const b2Vec2& mouse_position = ConvertVector(event_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)));

      if (!mouse_joint_)
      {
        b2AABB aabb;
        b2Vec2 delta(0.01f, 0.01f);
        aabb.lowerBound = mouse_position - delta;
        aabb.upperBound = mouse_position + delta;

        QueryCallback callback(mouse_position);
        world.QueryAABB(&callback, aabb);
        b2Fixture* fixture_in_aabb = callback.matching_fixture_;

        if (fixture_in_aabb)
        {
          b2Body* body = fixture_in_aabb->GetBody();
          b2MouseJointDef mouse_joint_def;
          mouse_joint_def.bodyA = world.ground_body_;
          mouse_joint_def.bodyB = body;
          mouse_joint_def.target = mouse_position;
          mouse_joint_def.maxForce = 1000.0f * body->GetMass();
          mouse_joint_ = static_cast<b2MouseJoint*>(world.CreateJoint(&mouse_joint_def));
          body->SetAwake(true);
        }
      }
      else
      {
        mouse_joint_->SetTarget(mouse_position);
      }
    }
    else if (mouse_joint_)
    {
      world.DestroyJoint(mouse_joint_);
      mouse_joint_ = nullptr;
    }
  }
}

