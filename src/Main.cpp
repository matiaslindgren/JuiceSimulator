#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <tclap/CmdLine.h>
#include "Grid.hpp"
#include "DebugDraw.hpp"
#include "World.hpp"


constexpr auto window_width = 900;
constexpr auto window_height = 600;
constexpr auto view_width = window_width/30.0f;
constexpr auto view_height = window_height/30.0f;
constexpr auto fps = 60;
constexpr auto antialiasinglevel = 4;
constexpr auto timeStep = 1.0f / 60.0f;
constexpr auto velocityIterations = 6;
constexpr auto positionIterations = 2;
constexpr auto particleIterations = 2;


void handleEvents(sf::RenderWindow& window)
{
  sf::Event event;
  while (window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
      window.close();
    } else if (event.type == sf::Event::MouseWheelScrolled &&
        event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
      sf::View view = window.getView();
      view.zoom(1.0f - event.mouseWheelScroll.delta/10.0f);
      window.setView(view);
    } else if (event.type == sf::Event::KeyPressed)
    {
      sf::View view = window.getView();
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
      window.setView(view);
    }
  }
}

int main(int argc, char** argv) {
  bool disableRendering;
  bool enablePhysicsDebug;
  unsigned int gameLoopLimit;
  try
  {
    TCLAP::CmdLine cmd("a game - work in progress", ' ', "0.1");
    TCLAP::SwitchArg renderSwitch(
        "r", "no-rendering", "Disable all rendering.",
        cmd, false);
    TCLAP::SwitchArg physicsDebugSwitch(
        "p", "physics-debug", "Render Box2D physics entities instead of SFML shapes and textures.",
        cmd, false);
    TCLAP::ValueArg<unsigned int> gameLoopLimitArg(
        "l", "loop-limit", "The amount of rendered window frames. The render window will be closed and application will terminate after this amount of frames have been rendered, regardless of application state.",
        false, 0, "unsigned integer", cmd);

    cmd.parse(argc, argv);

    disableRendering = renderSwitch.getValue();
    enablePhysicsDebug = physicsDebugSwitch.getValue();
    gameLoopLimit = gameLoopLimitArg.getValue();

  } catch (TCLAP::ArgException& e)
  {
    std::cerr << "ERROR while parsing command line arguments: "
      << e.error() << " for argument " << e.argId() << std::endl;
    return EXIT_FAILURE;
  }

  World world(0.0f, 8.0f, -10, 20, 20, -20);

  sf::RenderWindow window;
  if (!disableRendering)
  {
    sf::ContextSettings settings;
    settings.antialiasingLevel = antialiasinglevel;
    window.create(sf::VideoMode(window_width, window_height),
        "title todo", sf::Style::Default, settings);
    window.setFramerateLimit(fps);

    sf::View view = window.getDefaultView();
    view.setCenter(5-0.5, 5-0.5);
    view.setSize(view_width, view_height);
    window.setView(view);
  }

  DebugDraw debug_draw(window, sf::Vector2f(view_width/window_width, view_height/window_height));
  if (enablePhysicsDebug)
  {
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    /* flags += b2Draw::e_jointBit; */
    /* flags += b2Draw::e_aabbBit; */
    /* flags += b2Draw::e_pairBit; */
    /* flags += b2Draw::e_centerOfMassBit; */
    /* flags += b2Draw::e_particleBit; */
    debug_draw.SetFlags(flags);
    world.SetDebugDraw(&debug_draw);
  }


  {
    sf::Vector2f corners[4];

    for (auto j = 0; j < 10; j++)
    {
      for (auto i = 0; i < 100; i++)
      {
        float x = -10 + (rand() % 30);
        float y = -20 - (rand() % 300);
        corners[0] = sf::Vector2f(x, y);
        corners[1] = sf::Vector2f(x + 0.5, y);
        corners[2] = sf::Vector2f(x + 0.5, y + 0.5);
        corners[3] = sf::Vector2f(x, y + 0.5);
        world.CreateShape(corners, 4, b2_dynamicBody);
      }
    }

    {
      corners[0] = sf::Vector2f(-2.0f, 12.0f);
      corners[1] = sf::Vector2f(8.0f, 12.0f);
      corners[2] = sf::Vector2f(8.0f, 12.5f);
      corners[3] = sf::Vector2f(-2.0f, 12.5f);
      world.CreateShape(corners, 4, b2_staticBody);
    }
  }

  while (window.isOpen())
  {
    handleEvents(window);

    window.clear(sf::Color::White);

    world.Step(timeStep, velocityIterations, positionIterations, particleIterations, window);

    if (enablePhysicsDebug)
      world.DrawDebugData();

    window.display();

    if (gameLoopLimit && drawnFrames++ > gameLoopLimit)
      window.close();
  }

  return EXIT_SUCCESS;
}

