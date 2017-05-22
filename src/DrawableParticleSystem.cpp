#include "DrawableParticleSystem.hpp"
#include "LiquidDefinitions.hpp"
#include "Adapter.hpp"


DrawableParticleSystem::DrawableParticleSystem(const float& particle_radius)
  : particle_radius_(particle_radius),
    vertices_(sf::TrianglesFan, k_TriangleFanSegments + 2)
{
  const auto& triangle_fan_increment = 2.0f * b2_pi / k_TriangleFanSegments;

  unit_particle_circle.resize(k_TriangleFanSegments + 2);
  auto theta = 0.0f;
  for (auto i = 0; i < k_TriangleFanSegments + 2; i++)
  {
    unit_particle_circle[i] = particle_radius*sf::Vector2f(cosf(theta), sinf(theta));
    theta += triangle_fan_increment;
  }
}

void DrawableParticleSystem::Step(sf::RenderTarget& target, const b2ParticleSystem& particle_system)
{
  const b2Vec2* positions = particle_system.GetPositionBuffer();
  const b2ParticleColor* colors = particle_system.GetColorBuffer();
  const auto particle_count = particle_system.GetParticleCount();

  for (auto i = 0; i < particle_count; i++)
  {
    const sf::Color& sfml_color = ConvertColor(colors[i]);
    const sf::Vector2f& sfml_center = ConvertVector(positions[i]);
    for (auto j = 0; j < unit_particle_circle.size(); j++)
    {
      vertices_[j].position = sfml_center + unit_particle_circle[j];
      vertices_[j].color = sfml_color;
    }
    target.draw(vertices_);
  }
}

