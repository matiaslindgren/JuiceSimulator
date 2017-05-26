#ifndef DRAWABLEPARTICLESYSTEM_HPP
#define DRAWABLEPARTICLESYSTEM_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

static constexpr auto k_TriangleFanSegments = 10;

class DrawableParticleSystem
{
  public:
    DrawableParticleSystem()
      : vertices_(sf::TrianglesFan, k_TriangleFanSegments + 2)
    {
    }
    void GenerateVertices(const float& particle_radius);
    void Step(sf::RenderTarget&, const b2ParticleSystem&);
  private:
    sf::VertexArray vertices_;
    std::vector<sf::Vector2f> unit_particle_circle;
};


#endif // DRAWABLEPARTICLESYSTEM.HPP
