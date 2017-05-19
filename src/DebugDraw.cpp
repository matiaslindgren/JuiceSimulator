/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
* Copyright (c) 2013 Google, Inc.
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

// This source file has been altered from the original Box2D source file

#include <SFML/Graphics.hpp>
#include "DebugDraw.hpp"
#include "Adapter.hpp"

#include <iostream>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  sf::ConvexShape polygon(vertexCount);
  for (auto i = 0; i < vertexCount; i++)
    polygon.setPoint(i, convertVector(vertices[i]));
  polygon.setFillColor(sf::Color::Transparent);
  polygon.setOutlineColor(convertColor(color));
  polygon.setOutlineThickness(0.02f);
  m_window.draw(polygon);
}

void DebugDraw::DrawFlatPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  sf::ConvexShape polygon(vertexCount);
  for (auto i = 0; i < vertexCount; i++)
    polygon.setPoint(i, convertVector(vertices[i]));
  polygon.setFillColor(sf::Color(color.r, color.g, color.b, 255));
  m_window.draw(polygon);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
  sf::ConvexShape polygon(vertexCount);
  for (auto i = 0; i < vertexCount; i++)
    polygon.setPoint(i, convertVector(vertices[i]));
  polygon.setFillColor(convertColor(color));
  m_window.draw(polygon);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
  sf::CircleShape circle(radius);
  circle.setPosition(center.x, center.y);
  circle.setFillColor(sf::Color::Transparent);
  circle.setOutlineColor(convertColor(color));
}

float smoothstep(float x) { return x * x * (3 - 2 * x); }

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
  sf::CircleShape circle(radius);
  circle.setPosition(center.x, center.y);
  circle.setFillColor(convertColor(color));
  m_window.draw(circle);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
  sf::VertexArray line(sf::Lines, 2);
  line[0].position = convertVector(p1);
  line[1].position = convertVector(p2);
  line[0].color = sf::Color(convertColor(color));
  line[1].color = sf::Color(convertColor(color));
  m_window.draw(line);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
  constexpr float k_axisScale = 1.4f;
  const sf::Vector2f& p1 = convertVector(xf.p);
  sf::Vector2f p2;

  sf::VertexArray line(sf::Lines, 4);
  line[0] = p1;
  line[0].color = sf::Color(1, 0, 0);
  p2 = p1 + k_axisScale * convertVector(xf.q.GetXAxis());
  line[1] = p2;
  line[1].color = sf::Color(1, 0, 0);
  line[2] = p1;
  line[2].color = sf::Color(0, 1, 0);
  p2 = p1 + k_axisScale * convertVector(xf.q.GetYAxis());
  line[3] = p2;
  line[3].color = sf::Color(0, 1, 0);
  m_window.draw(line);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
  this->DrawCircle(p, size, color);
}

void DebugDraw::DrawString(int x, int y, const char *string, ...)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << "called DrawString with x y " << std::endl;
}

void DebugDraw::DrawString(const b2Vec2& p, const char *string, ...)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << "called DrawString with vector" << std::endl;
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
  sf::VertexArray quad(sf::Quads, 4);
  quad[0] = sf::Vector2f(aabb->lowerBound.x, -aabb->lowerBound.y);
  quad[1] = sf::Vector2f(aabb->upperBound.x, -aabb->lowerBound.y);
  quad[2] = sf::Vector2f(aabb->upperBound.x, -aabb->upperBound.y);
  quad[3] = sf::Vector2f(aabb->lowerBound.x, -aabb->upperBound.y);
  for (auto i = 0; i < 4; i++)
    quad[i].color = convertColor(c);
}

float ComputeFPS()
{
  /* static bool debugPrintFrameTime = false; */
  /* static int lastms = 0; */
  /* int curms = glutGet(GLUT_ELAPSED_TIME); */
  /* int delta = curms - lastms; */
  /* lastms = curms; */

  /* static float dsmooth = 16; */
  /* dsmooth = (dsmooth * 30 + delta) / 31; */

  /* if ( debugPrintFrameTime ) */
  /* { */
  /* #ifdef ANDROID */
  /*  __android_log_print(ANDROID_LOG_VERBOSE, "Testbed", "msec = %f", dsmooth); */
  /* #endif */
  /* } */

  /* return dsmooth; */
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << "called ComputeFPS" << std::endl;
  return -1.0f;
}

void DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count)
{
  std::cout << "NOT IMPLEMENTED: ";
  std::cout << "called DrawParticles" << std::endl;
  /* static unsigned int particle_texture = 0; */

  /* if (!particle_texture || */
  /*    !glIsTexture(particle_texture)) // Android deletes textures upon sleep etc. */
  /* { */
  /*  // generate a "gaussian blob" texture procedurally */
  /*  glGenTextures(1, &particle_texture); */
  /*  b2Assert(particle_texture); */
  /*  const int TSIZE = 64; */
  /*  unsigned char tex[TSIZE][TSIZE][4]; */
  /*  for (int y = 0; y < TSIZE; y++) */
  /*  { */
  /*    for (int x = 0; x < TSIZE; x++) */
  /*    { */
  /*      float fx = (x + 0.5f) / TSIZE * 2 - 1; */
  /*      float fy = (y + 0.5f) / TSIZE * 2 - 1; */
  /*      float dist = sqrtf(fx * fx + fy * fy); */
  /*      unsigned char intensity = (unsigned char)(dist <= 1 ? smoothstep(1 - dist) * 255 : 0); */
  /*      tex[y][x][0] = tex[y][x][1] = tex[y][x][2] = 128; */
  /*      tex[y][x][3] = intensity; */
  /*    } */
  /*  } */
  /*  glEnable(GL_TEXTURE_2D); */
  /*  glBindTexture(GL_TEXTURE_2D, particle_texture); */
  /*  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); */
  /*  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); */
  /*  glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); */
  /*  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */
  /*  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); */
  /*  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TSIZE, TSIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex); */
  /*  glDisable(GL_TEXTURE_2D); */

  /*  glEnable(GL_POINT_SMOOTH); */
  /* } */

  /* glEnable(GL_TEXTURE_2D); */
  /* glBindTexture(GL_TEXTURE_2D, particle_texture); */

  /* #ifdef __ANDROID__ */
  /*  glEnable(GL_POINT_SPRITE_OES); */
  /*  glTexEnvf(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE); */
  /* #else */
  /*  glEnable(GL_POINT_SPRITE); */
  /*  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE); */
  /* #endif */

  /* const float particle_size_multiplier = 3;  // because of falloff */
  /* glPointSize(radius * currentscale * particle_size_multiplier); */

  /* glEnable(GL_BLEND); */
  /* glBlendFunc(GL_SRC_ALPHA, GL_ONE); */

  /* glEnableClientState(GL_VERTEX_ARRAY); */
  /* glVertexPointer(2, GL_FLOAT, 0, &centers[0].x); */
  /* if (colors) */
  /* { */
  /*  glEnableClientState(GL_COLOR_ARRAY); */
  /*  glColorPointer(4, GL_UNSIGNED_BYTE, 0, &colors[0].r); */
  /* } */
  /* else */
  /* { */
  /*  glColor4f(1, 1, 1, 1); */
  /* } */

  /* glDrawArrays(GL_POINTS, 0, count); */

  /* glDisableClientState(GL_VERTEX_ARRAY); */
  /* if (colors) glDisableClientState(GL_COLOR_ARRAY); */

  /* glDisable(GL_BLEND); */
  /* glDisable(GL_TEXTURE_2D); */
  /* #ifdef __ANDROID__ */
  /*  glDisable(GL_POINT_SPRITE_OES); */
  /* #endif */
}

