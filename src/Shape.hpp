#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <memory>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


#include <iostream>
class Polygon : public sf::Drawable
{
  public:
    Polygon(const sf::Texture&, const sf::FloatRect&);

    const b2BodyDef* getB2BodyDefinition() const
    {
      return &m_body_def;
    }

    const b2PolygonShape* getB2Polygon() const
    {
      return &m_shape;
    }

    void setB2Body(b2Body* body)
    {
      m_body = body;
    }

    const sf::Vector2f& getPosition() const
    {
      return m_vertices[0].position;
    }

    void applyPhysics();

    // TODO temp, debug
    sf::Vector2f getB2Position() const
    {
      return sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y);
    }

    friend std::ostream& operator<<(std::ostream&, const Polygon&);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    static constexpr auto shape_scale_x = 1.0f;
    static constexpr auto shape_scale_y = -1.0f;
    static constexpr auto vertex_count = 4;

    b2PolygonShape m_shape;
    b2BodyDef m_body_def;
    b2Body* m_body;
    const sf::Texture& m_texture;
    sf::VertexArray m_vertices;
};

#endif // POLYGON.HPP
