#ifndef GAMEITEM_HPP
#define GAMEITEM_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

// Represents a drawable element which the user can interact with.
// If void* b2Fixture::m_userData does not point to NULL, it must point to
// one GameItem instance.
class GameItem
{
  public:
    GameItem(const b2Vec2 vertices[],
             const unsigned int& vertex_count,
             const sf::Color& color);

    // Apply a transform to the vertices of this game item.
    void ApplyTransform(const b2Transform&);

    // Draw vertices of this game item onto the render target given
    // as parameter.
    void Draw(const sf::RenderTarget&);

    // Methods called by the EventHandler when the graphical element
    // represented by this game item is interacted with.
    void Click();
    void Press();
    void Release();

  private:
    sf::VertexArray vertices_;
    sf::Texture* texture_;
    sf::Color color_;
};

#endif // GAMEITEM_HPP
