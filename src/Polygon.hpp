#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>


class Polygon : public sf::Drawable, public sf::Transformable
{
  public:
    Polygon(sf::Vector2f* corners, b2BodyType body_type);

    const b2BodyDef* getB2BodyDefinition() const
    {
      return &m_body_def;
    }

    const b2FixtureDef* getB2FixtureDefinition() const
    {
      return &m_fixture_def;
    }

    void setB2Body(b2Body* body)
    {
      m_body = body;
    }

    void applyPhysics();

    // TODO temp, debug
    sf::Vector2f getB2Position() const
    {
      return convertVector(m_body->GetWorldCenter());
    }

    friend std::ostream& operator<<(std::ostream&, const Polygon&);

  private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    static constexpr auto vertex_count = 4;

    b2BodyDef m_body_def;
    b2PolygonShape m_shape;
    b2FixtureDef m_fixture_def;
    b2Body* m_body;
    sf::VertexArray m_vertices;
};


#endif // POLYGON.HPP
