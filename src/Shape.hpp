#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <memory>
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "TextBox.hpp"


#include <iostream>
class Polygon : public sf::Drawable, public sf::Transformable
{
  public:
    Polygon(const sf::Texture&, const sf::FloatRect&, b2BodyType body_type);

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

    void applyPhysics();

    // TODO temp, debug
    sf::Vector2f getB2Position() const
    {
      return sf::Vector2f(m_body->GetPosition().x, m_body->GetPosition().y);
    }

    friend std::ostream& operator<<(std::ostream&, const Polygon&);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    static constexpr auto vertex_count = 4;

    b2PolygonShape m_shape;
    b2BodyDef m_body_def;
    b2Body* m_body;
    const sf::Texture& m_texture;
    sf::VertexArray m_vertices;
    TextBox m_text_box;
};


#endif // POLYGON.HPP
