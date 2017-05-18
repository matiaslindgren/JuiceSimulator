#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Grid.hpp"
#include "World.hpp"
#include "StateManager.hpp"

constexpr auto width = 900;
constexpr auto height = 600;
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


int main() {
  StateManager state_manager;
  sf::ContextSettings settings;
  World world(2.0f, 10.0f);

  settings.antialiasingLevel = 4;
  sf::RenderWindow window(sf::VideoMode(width, height), "title todo", sf::Style::Default, settings);
  window.setFramerateLimit(10);

  sf::View view = window.getDefaultView();
  view.setCenter(5-0.1, 5-0.1);
  view.setSize(10, 10);
  window.setView(view);

  Grid grid;

  sf::Texture wood;
  if (!wood.loadFromFile("media/img/wood.jpg"))
    return EXIT_FAILURE;
  sf::Texture pulp;
  if (!pulp.loadFromFile("media/img/pulp-fiction.jpg"))
    return EXIT_FAILURE;
  world.CreateShape(pulp, 1.0f, sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f), b2_dynamicBody);
  world.CreateShape(wood, 1.0f, sf::FloatRect(0.0f, 3.0f, 5.0f, 1.0f), b2_staticBody);

  state_manager.push_state(StateName(menu));

  cs(state_manager, world);

  bool ready = true;
  while (window.isOpen())
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
          } else if (event.type == sf::Event::MouseMoved && !ready)
          {
            sf::Vector2i mouse = sf::Mouse::getPosition(window);
            std::cout << "mouse: " << mouse.x << ", " << mouse.y << std::endl;
            sf::Vector2f world = window.mapPixelToCoords(mouse);
            std::cout << "world: " << world.x << ", " << world.y << std::endl;
            ready = true;
          }

      }

      window.clear(sf::Color::White);
      window.draw(grid);

      world.TimeStep(timeStep, velocityIterations, positionIterations);
      int i = 1;
      for (auto shape : world.getShapeList())
      {
        /* std::cout << i << ": " << shape.getB2Position().x << ", " << shape.getB2Position().y << std::endl; */
        window.draw(shape);
        i++;
      }

      window.display();
      ready = false;
  }
  state_manager.pop_state();
  cs(state_manager, world);

  return EXIT_SUCCESS;
}

