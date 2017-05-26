#include <cstdlib>
#include <cmath>
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


int main(int argc, char** argv)
{
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
  static constexpr auto kAntialiasinglevel = 2;

  static constexpr auto kParticleGravityScale = 6.0f;
  static constexpr auto kParticleDensity = 0.4f;
  static constexpr auto kParticleRadius = 0.3f;

  sf::RenderWindow window;
  sf::ContextSettings settings;
  settings.antialiasingLevel = kAntialiasinglevel;

  window.create(sf::VideoMode(800, 600), "Juice Simulator", sf::Style::Default, settings);

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

  // Create 3 juice dispensers
  for (int i = 0; i < 3; i++)
  {
    b2Vec2 dispenser_position = b2Vec2(-15 + 15*i, -5);
    // Create 4 buttons with 4 different emit rates
    for (int j = 0; j < 4; j++)
    {
      b2ParticleColor liquid_color = colors[i];
      sf::Color button_color = (j > 0) ?
        sf::Color(255 - (j/3.0f)*abs(255 - liquid_color.r),
                  255 - (j/3.0f)*abs(255 - liquid_color.g),
                  255 - (j/3.0f)*abs(255 - liquid_color.b)) :
        sf::Color(160, 160, 160, 255);
      b2Vec2 button_position = dispenser_position + b2Vec2(0, -2.1*j);
      auto new_emit_rate = j*std::pow(8, j);
      world.CreateDispenserButton(i, button_position, new_emit_rate, button_color);
    }
    world.CreateDispenser(Juice(colors[i]), dispenser_position);
    // Create cup under dispenser
    world.CreateItem(ItemTypes(k_Cup), b2Vec2(-17 + 15*i, 0), b2Vec2(3, 7));
  }

  // Create a sponge and a little shelf for it
  world.CreateItem(ItemTypes(k_Surface), b2Vec2(-35, -20), b2Vec2(10, 1));
  world.CreateItem(ItemTypes(k_Box), b2Vec2(-33, -25), b2Vec2(6, 3));

  // Create the counter
  world.CreateItem(ItemTypes(k_Surface), b2Vec2(-30, 15), b2Vec2(60, 3));

  constexpr auto k_TimeStep = 1.0f / 60.0f;
  constexpr auto k_VelocityIterations = 6;
  constexpr auto k_PositionIterations = 3;
  constexpr auto k_ParticleIterations = 3;

  // Track time for counting frames per second
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

    // Advance the whole world by one time step
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

