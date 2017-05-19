#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <tclap/CmdLine.h>
#include "Grid.hpp"
#include "DebugDraw.hpp"
#include "World.hpp"
#include "StateManager.hpp"


constexpr auto window_width = 900;
constexpr auto window_height = 600;
constexpr auto fps = 60;
constexpr auto antialiasinglevel = 4;
constexpr auto timeStep = 1.0f / 60.0f;
constexpr auto velocityIterations = 6;
constexpr auto positionIterations = 2;


void cs(const StateManager& sm, const World& w) {
  std::cout << (sm.has_state() ? sm.active_state_name() : "no state") << std::endl;
  std::cout << "World contains " << w.getShapeCount() << " shapes" << std::endl;
}

static int n = 0;
void print_pos(const float& x, const float& y) {
  n++;
  std::cout << n << ": " << x << ", " << y << std::endl;
}

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
    } else if (event.type == sf::Event::MouseMoved)
    {
      sf::Vector2i mouse = sf::Mouse::getPosition(window);
      /* std::cout << "mouse: " << mouse.x << ", " << mouse.y << std::endl; */
      sf::Vector2f world = window.mapPixelToCoords(mouse);
      /* std::cout << "world: " << world.x << ", " << world.y << std::endl; */
    }
  }
}

int main(int argc, char** argv) {
  bool disableRendering;
  bool enablePhysicsDebug;
  try
  {
    TCLAP::CmdLine cmd("a game - work in progress", ' ', "0.1");
    TCLAP::SwitchArg renderSwitch(
        "r", "no-rendering", "Disable all rendering.",
        cmd, false);
    TCLAP::SwitchArg physicsDebugSwitch(
        "p", "physics-debug", "Render Box2D physics entities instead of SFML shapes and textures.",
        cmd, false);

    cmd.parse(argc, argv);

    disableRendering = renderSwitch.getValue();
    enablePhysicsDebug = physicsDebugSwitch.getValue();

  } catch (TCLAP::ArgException& e)
  {
    std::cerr << "ERROR while parsing command line arguments: "
      << e.error() << " for argument " << e.argId() << std::endl;
    return EXIT_FAILURE;
  }

  StateManager state_manager;
  World world(0.0f, 10.0f);

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
    view.setSize(10, 10);
    window.setView(view);
  }

  DebugDraw debug_draw(window);
  if (enablePhysicsDebug)
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

  Grid grid;

  sf::Texture wood;
  if (!wood.loadFromFile("media/img/wood.jpg"))
    return EXIT_FAILURE;
  sf::Texture pulp;
  if (!pulp.loadFromFile("media/img/pulp-fiction.jpg"))
    return EXIT_FAILURE;
  world.CreateShape(pulp, 1.0f, sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f), b2_dynamicBody);
  world.CreateShape(wood, 1.0f, sf::FloatRect(0.0f, 3.0f, 5.0f, 0.1f), b2_staticBody);

  state_manager.push_state(StateName(menu));
  cs(state_manager, world);

  while (window.isOpen())
  {
    handleEvents(window);

    window.clear(sf::Color::White);
    window.draw(grid);

    world.TimeStep(timeStep, velocityIterations, positionIterations);
    for (auto shape : world.getShapeList())
      window.draw(shape);

    world.DrawDebugData();
    window.display();
  }

  state_manager.pop_state();
  cs(state_manager, world);

  return EXIT_SUCCESS;
}

