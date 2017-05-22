#ifndef DRAWABLEPARTICLESYSTEM_HPP
#define DRAWABLEPARTICLESYSTEM_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

static constexpr auto k_TriangleFanSegments = 10;

class DrawableParticleSystem
{
  public:
    DrawableParticleSystem(const float& radius);
    void Step(sf::RenderTarget&, const b2ParticleSystem&);
    const float& get_particle_radius() const
    {
      return particle_radius_;
    }
  private:
    sf::VertexArray vertices_;
    std::vector<sf::Vector2f> unit_particle_circle;
    const float particle_radius_;
};


#endif // DRAWABLEPARTICLESYSTEM.HPP
