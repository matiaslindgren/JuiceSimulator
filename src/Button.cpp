#include "Button.hpp"

static constexpr float k_segments = 16.0f;
static constexpr float k_increment = 2.0f * b2_pi / k_segments;

Button::Button(const b2Vec2& center,
               const float& radius,
               std::function<void()>& toggle)
  : vertices_(sf::TrianglesFan, (int)k_segments + 2)
{
  toggle_ = toggle;
  float theta = 0.0f;
  sf::Vector2f sfml_center = ConvertVector(center);
  for (auto i = 0; i < vertices_.getVertexCount(); i++)
  {
    vertices_[i].position = sfml_center + radius*sf::Vector2f(cosf(theta), sinf(theta));
    vertices_[i].color = sf::Color::Red;
    theta += k_increment;
  }
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  target.draw(vertices_, states);
}

