#include <cstdlib>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <tclap/CmdLine.h>
#include "Adapter.hpp"
#include "DebugDraw.hpp"
#include "EventHandler.hpp"
#include "World.hpp"
#include "WorldCallbacks.hpp"
#include "LiquidDefinitions.hpp"


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

  World world(0.0f, 9.81f, -50, 50, 50, -50);

  DestructionListener destruction_listener;
  world.SetDestructionListener(&destruction_listener);

  EventHandler event_handler;

  static constexpr auto kWindowWidth = 900;
  static constexpr auto kWindowHeight = 600;
  static constexpr auto kViewWidth = kWindowWidth/10.0f;
  static constexpr auto kViewHeight = kWindowHeight/10.0f;
  static constexpr auto kFps = 60;
  static constexpr auto kAntialiasinglevel = 4;

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

  DebugDraw debug_draw(window);
  if (enable_physics_debug)
  {
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_particleBit;

    /* flags += b2Draw::e_jointBit; */
    /* flags += b2Draw::e_aabbBit; */
    /* flags += b2Draw::e_pairBit; */
    /* flags += b2Draw::e_centerOfMassBit; */

    debug_draw.SetFlags(flags);
    world.set_debug_draw(&debug_draw);
  }


  for (auto i = 0; i < 3; i++)
  {
    world.CreateDispenser(Juice(b2ParticleColor(250 - 50*i, 50*i, 250*i%2, 50)), b2Vec2(-25 + 30*i, -5));
    world.CreateItem(ItemTypes(k_Cup), b2Vec2(-27 + 30*i, 0));
  }

  world.CreateItem(ItemTypes(k_Counter), b2Vec2(-30, 12));
  world.CreateItem(ItemTypes(k_Counter), b2Vec2(0, 12));
  world.CreateItem(ItemTypes(k_Counter), b2Vec2(30, 12));

  constexpr auto k_TimeStep = 1.0f / 30.0f;
  constexpr auto k_VelocityIterations = 6;
  constexpr auto k_PositionIterations = 3;
  constexpr auto k_ParticleIterations = 3;

  unsigned int drawnFrames = 0;
  while (window.isOpen())
  {
    event_handler.HandleEvents(
        window,
        (enable_physics_debug) ? &debug_draw : nullptr,
        world);

    window.clear(sf::Color::White);

    world.Step(k_TimeStep, k_VelocityIterations,
               k_PositionIterations, k_ParticleIterations,
               window, disable_sfml_graphics);

    if (enable_physics_debug)
      world.DrawDebugData();

    window.display();

    if (game_loop_limit && drawnFrames++ > game_loop_limit)
      window.close();
  }

  return EXIT_SUCCESS;
}

