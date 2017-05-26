#ifndef GAMEITEM_HPP
#define GAMEITEM_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <vector>


// Represents a drawable element which the user can interact with.
// If void* b2Fixture::m_userData does not point to NULL, it must point to
// one GameItem instance.
class GameItem
{
  public:
    // Resize the vertex array and update the positions and color
    // of all vertices from the given constructor parameters.
    // The y axis is flipped for each vertex.
    GameItem(const b2Vec2 b2_vertices[],
             const unsigned int& vertex_count,
             const sf::PrimitiveType& sfml_vertex_primitive,
             const sf::Color& color,
             const sf::RenderStates& render_states=sf::RenderStates::Default);

    // Apply a transform to the vertices of this game item.
    void ApplyTransform(const b2Transform&);

    // Draw vertices of this game item onto the render target given
    // as parameter.
    void Draw(sf::RenderTarget&);

  private:
    sf::VertexArray draw_vertices_;
    std::vector<b2Vec2> shape_vertices_;
    sf::RenderStates render_states_;
};

#endif // GAMEITEM_HPP

