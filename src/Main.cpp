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

  static constexpr auto kFps = 60;
  static constexpr auto k_ViewScale = 0.1f;
  static constexpr auto kAntialiasinglevel = 4;
  static constexpr auto kParticleGravityScale = 1.0f;
  static constexpr auto kParticleDensity = 1.0f;
  static constexpr auto kParticleRadius = 0.3f;

  sf::RenderWindow window;
  sf::ContextSettings settings;
  settings.antialiasingLevel = kAntialiasinglevel;

  bool found_valid_video_mode = false;
  for (auto& video_mode : sf::VideoMode::getFullscreenModes())
  {
    if (video_mode.isValid())
    {
      found_valid_video_mode = true;
      window.create(video_mode, "splash splosh", sf::Style::Fullscreen, settings);
      break;
    }
  }
  if (!found_valid_video_mode)
  {
    std::cerr << "Could not find valid video mode for full screen" << std::endl;
    window.create(sf::VideoMode(), "splash splosh", sf::Style::Default, settings);
  }

  window.setFramerateLimit(kFps);

  const sf::Vector2u& window_size = window.getSize();
  const sf::Vector2f& view_size = sf::Vector2f(k_ViewScale*window_size.x, k_ViewScale*window_size.y);
  sf::View view = window.getDefaultView();
  view.setCenter(0, 0);
  view.setSize(view_size);
  window.setView(view);

  World world(0.0f, 10.0f, -view_size.y/1.5f, view_size.x/1.5f, view_size.y/1.5f, -view_size.x/1.5f);

  DestructionListener destruction_listener;
  world.SetDestructionListener(&destruction_listener);

  EventHandler event_handler;

  DebugDraw debug_draw(window);
  if (enable_physics_debug)
  {
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    flags += b2Draw::e_particleBit;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_aabbBit;
    /* flags += b2Draw::e_pairBit; */
    /* flags += b2Draw::e_centerOfMassBit; */
    debug_draw.SetFlags(flags);
    world.set_debug_draw(&debug_draw);
  }

  sf::Texture dispenser_texture;
  if (!dispenser_texture.loadFromFile("media/img/dispenser.jpg"))
  {
    std::cerr << "Loading dispenser texture failed" << std::endl;
    return EXIT_FAILURE;
  }

  world.CreateParticleSystem(kParticleGravityScale,
                             kParticleDensity,
                             kParticleRadius);
  b2ParticleColor colors[] = {
    b2ParticleColor(250, 20, 20, 50),
    b2ParticleColor(20, 250, 20, 50),
    b2ParticleColor(20, 20, 250, 50),
  };
  for (auto i = 0; i < 3; i++)
  {
    world.CreateDispenser(Juice(colors[i]), b2Vec2(-20 + 20*i, -5), &dispenser_texture);
    world.CreateItem(ItemTypes(k_Cup), b2Vec2(-5 + i*15, 0), b2Vec2(3, 7));
  }

  world.CreateItem(ItemTypes(k_Surface), b2Vec2(-30, -15), b2Vec2(10, 3));
  world.CreateItem(ItemTypes(k_Box), b2Vec2(-25, -20), b2Vec2(6, 3));

  world.CreateItem(ItemTypes(k_Surface), b2Vec2(-30, 15), b2Vec2(60, 3));

  constexpr auto k_TimeStep = 1.0f / 60.0f;
  constexpr auto k_VelocityIterations = 6;
  constexpr auto k_PositionIterations = 3;
  constexpr auto k_ParticleIterations = 3;

  sf::Clock clock;
  int last_time = 0;
  unsigned int drawnFrames = 0;

  while (!disable_rendering && window.isOpen())
  {
    if (enable_physics_debug)
      event_handler.HandleEvents(window, &debug_draw, world);
    else
      event_handler.HandleEvents(window, nullptr, world);

    window.clear(sf::Color::White);

    world.Step(k_TimeStep, k_VelocityIterations,
               k_PositionIterations, k_ParticleIterations,
               window, disable_sfml_graphics);

    int current_time = clock.getElapsedTime().asMilliseconds();
    int delta_time = current_time - last_time;
    last_time = current_time;
    if (enable_physics_debug)
      world.DrawDebugData(1000/delta_time);

    window.display();

    if (game_loop_limit && drawnFrames++ > game_loop_limit)
      window.close();
  }

  return EXIT_SUCCESS;
}

