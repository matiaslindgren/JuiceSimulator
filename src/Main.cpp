#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <tclap/CmdLine.h>
#include "DebugDraw.hpp"
#include "World.hpp"
#include "WorldCallbacks.hpp"
#include "LiquidDefinitions.hpp"


void HandleEvents(sf::RenderWindow& window, DebugDraw& debug_draw)
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
    } else if (event.type == sf::Event::MouseMoved)
    {
      const sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
      debug_draw.set_mouse_coordinates(mouse_position.x, mouse_position.y);
    }
  }
}

int main(int argc, char** argv) {
  bool disable_rendering;
  bool enable_physics_debug;
  bool disable_sfml_graphics;
  unsigned int game_loop_limit;
  try
  {
    TCLAP::CmdLine cmd("a game - work in progress", ' ', "0.1");
    TCLAP::SwitchArg render_switch(
        "r", "no-rendering", "Disable all rendering.",
        cmd, false);
    TCLAP::SwitchArg physics_debug_switch(
        "p", "physics-debug", "Render Box2D physics entities.",
        cmd, false);
    TCLAP::SwitchArg disable_sfml_graphics_switch(
        "d", "no-sfml-graphics", "Disable SFML graphics rendering. Does not affect physics debug rendering.",
        cmd, false);
    TCLAP::ValueArg<unsigned int> game_loop_limit_arg(
        "l", "loop-limit", "The amount of rendered window frames. The render window will be closed and application will terminate after this amount of frames have been rendered, regardless of application state.",
        false, 0, "unsigned integer", cmd);

    cmd.parse(argc, argv);

    disable_rendering = render_switch.getValue();
    enable_physics_debug = physics_debug_switch.getValue();
    game_loop_limit = game_loop_limit_arg.getValue();
    disable_sfml_graphics = disable_sfml_graphics_switch.getValue();

  } catch (TCLAP::ArgException& e)
  {
    std::cerr << "ERROR while parsing command line arguments: "
      << e.error() << " for argument " << e.argId() << std::endl;
    return EXIT_FAILURE;
  }

  World world(0.0f, 8.0f, -50, 50, 50, -50);

  DestructionListener destruction_listener;
  world.SetDestructionListener(&destruction_listener);

  constexpr auto kWindowWidth = 900;
  constexpr auto kWindowHeight = 600;
  constexpr auto kViewWidth = kWindowWidth/30.0f;
  constexpr auto kViewHeight = kWindowHeight/30.0f;
  constexpr auto kFps = 60;
  constexpr auto kAntialiasinglevel = 4;


  sf::RenderWindow window;
  if (!disable_rendering)
  {
    sf::ContextSettings settings;
    settings.antialiasingLevel = kAntialiasinglevel;
    window.create(sf::VideoMode(kWindowWidth, kWindowHeight),
        "title todo", sf::Style::Default, settings);
    window.setFramerateLimit(kFps);

    sf::View view = window.getDefaultView();
    view.setCenter(5-0.5, 5-0.5);
    view.setSize(kViewWidth, kViewHeight);
    window.setView(view);
  }

  DebugDraw debug_draw(window, sf::Vector2f(kViewWidth/kWindowWidth, kViewHeight/kWindowHeight));
  if (enable_physics_debug)
  {
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    flags += b2Draw::e_pairBit;
    flags += b2Draw::e_centerOfMassBit;
    flags += b2Draw::e_particleBit;
    debug_draw.SetFlags(flags);
    world.SetDebugDraw(&debug_draw);
  }


  {
    sf::Vector2f corners[4];

    for (auto i = 0; i < 100; i++)
    {
      auto x = 5 + rand() % 16 - 8;
      auto y = -5 + rand() % 16 - 8;
      corners[0] = sf::Vector2f(x, y);
      corners[1] = sf::Vector2f(x + 0.5, y);
      corners[2] = sf::Vector2f(x + 0.5, y + 0.5);
      corners[3] = sf::Vector2f(x, y + 0.5);
      world.CreateShape(corners, 4, b2_dynamicBody);
    }

    {
      corners[0] = sf::Vector2f(-5.0f, 12.0f);
      corners[1] = sf::Vector2f(9.0f, 12.0f);
      corners[2] = sf::Vector2f(9.0f, 12.5f);
      corners[3] = sf::Vector2f(-5.0f, 12.5f);
      world.CreateShape(corners, 4, b2_staticBody);
    }

  }

  constexpr auto kTimeStep = 1.0f / 60.0f;
  constexpr auto kVelocityIterations = 6;
  constexpr auto kPositionIterations = 2;
  constexpr auto kParticleIterations = 2;

  unsigned int drawnFrames = 0;
  while (window.isOpen())
  {
    HandleEvents(window, debug_draw);

    window.clear(sf::Color::White);

    world.Step(kTimeStep, kVelocityIterations, kPositionIterations, kParticleIterations, window, disable_sfml_graphics);

    if (enable_physics_debug)
      world.DrawDebugData();

    window.display();

    if (game_loop_limit && drawnFrames++ > game_loop_limit)
      window.close();
  }

  return EXIT_SUCCESS;
}

