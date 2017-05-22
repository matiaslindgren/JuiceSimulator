#ifndef LIQUIDDEFINITIONS_HPP
#define LIQUIDDEFINITIONS_HPP
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

/* // box2d source
struct b2ParticleGroupDef
{
		flags = 0;
		groupFlags = 0;
		position = b2Vec2_zero;
		angle = 0;
		linearVelocity = b2Vec2_zero;
		angularVelocity = 0;
		color = b2ParticleColor_zero;
		strength = 1;
		shape = NULL;
		shapes = NULL;
		shapeCount = 0;
		stride = 0;
		particleCount = 0;
		positionData = NULL;
		lifetime = 0.0f;
		userData = NULL;
		group = NULL;
}
*/

struct Water : public b2ParticleGroupDef
{
	Water()
	{
		flags = b2_waterParticle | b2_colorMixingParticle;
    color = b2ParticleColor(200, 200, 200, 220);
	}
};

struct Juice : public Water
{
	Juice(const b2ParticleColor& init_color)
  {
    color = init_color;
  }
};

#endif // LIQUIDDEFINITIONS.HPP
