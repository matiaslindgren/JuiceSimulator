#include <iostream>
#include <array>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "Main.hpp"
#include "Axes.hpp"
#include "World.hpp"
#include "StateManager.hpp"

constexpr auto world_scale_x = 100.0f;
constexpr auto world_scale_y = 100.0f;

const sf::Transform SCALE_WORLD(
    world_scale_x, 0, 0,
    0, world_scale_y, 0,
    0, 0, 1
);
const sf::Transform FLIP_Y(
    0, 0, 0,
    0, -1, 0,
    0, 0, 1
);

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

static constexpr auto width = 800;
static constexpr auto height = 600;

void draw_axes(sf::RenderWindow& window) {
  sf::Vertex y[2];
  y[0] = sf::Vertex(sf::Vector2f(0, -10));
  y[1] = sf::Vertex(sf::Vector2f(0, 10));
  y[0].color = sf::Color::Black;
  y[1].color = sf::Color::Black;
  sf::Vertex x[2];
  x[0] = sf::Vertex(sf::Vector2f(-10, 0));
  x[1] = sf::Vertex(sf::Vector2f(10, 0));
  x[0].color = sf::Color::Black;
  x[1].color = sf::Color::Black;
  window.draw(y, 2, sf::Lines);
  window.draw(x, 2, sf::Lines);
}

int main() {
  StateManager state_manager;
  sf::ContextSettings settings;
  World world(0.0f, -10.0f);

	constexpr auto timeStep = 1.0f / 60.0f;
	constexpr auto velocityIterations = 6;
	constexpr auto positionIterations = 2;
  constexpr auto view_size = 4.0f;

  settings.antialiasingLevel = 4;
  sf::RenderWindow window(sf::VideoMode(width, height), "title todo", sf::Style::Default, settings);
  window.setFramerateLimit(1);

  sf::View view = window.getDefaultView();
  view.setCenter(view_size/2.2f, view_size/2.2f);
  view.setSize(view_size, -view_size);
  window.setView(view);

  sf::Texture texture;
  texture.loadFromFile("wood.jpg");
  world.CreateShape(texture, 1.0f, sf::FloatRect(1.0f, 3.0f, 1.0f, 1.0f));
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
            switch(event.key.code)
            {
              case sf::Keyboard::Up:
                view.move(0.0f, -10.0f);
                break;
              case sf::Keyboard::Left:
                view.move(-10.0f, 0.0f);
                break;
              case sf::Keyboard::Down:
                view.move(0.0f, 10.0f);
                break;
              case sf::Keyboard::Right:
                view.move(10.0f, 0.0f);
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

      world.TimeStep(timeStep, velocityIterations, positionIterations);
      for (auto shape : world.getShapeList())
      {
        std::cout << shape << std::endl;
        window.draw(shape);
      }

      draw_axes(window);
      window.display();
      ready = false;
  }
  state_manager.pop_state();
  cs(state_manager, world);

  return 0;
}

