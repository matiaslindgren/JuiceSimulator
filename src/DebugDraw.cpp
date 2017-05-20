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
#include "math.h"
#include "DebugDraw.hpp"
#include "Adapter.hpp"
#include "TextBox.hpp"

#include <iostream>

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertex_count, const b2Color& color)
{
  sf::VertexArray sfml_vertices(sf::LinesStrip, vertex_count);
  const sf::Color& sfml_color = ConvertColor(color);
  for (auto i = 0; i < vertex_count; i++)
  {
    sfml_vertices[i].position = ConvertVector(vertices[i]);
    sfml_vertices[i].color = sfml_color;
  }
  window_.draw(sfml_vertices);
}

void DebugDraw::DrawFlatPolygon(const b2Vec2* vertices, int32 vertex_count, const b2Color& color)
{
  std::cout << "DrawFlatPolygon NOT IMPLEMENTED, calling DrawPolygon" << std::endl;
  DrawPolygon(vertices, vertex_count, color);
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertex_count, const b2Color& color)
{
  sf::VertexArray sfml_vertices(sf::TrianglesFan, vertex_count);
  const sf::Color& sfml_color = ConvertColor(color);
  for (auto i = 0; i < vertex_count; i++)
  {
    sfml_vertices[i].position = ConvertVector(vertices[i]);
    sfml_vertices[i].color = sfml_color;
  }
  window_.draw(sfml_vertices);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	constexpr float k_segments = 16.0f;
	constexpr float k_increment = 2.0f * b2_pi / k_segments;
  float theta = 0.0f;
  sf::VertexArray sfml_vertices(sf::LinesStrip, k_segments+1);
  const sf::Color& sfml_color = ConvertColor(color);
  sf::Vector2f sfml_center = ConvertVector(center);
  for (auto i = 0; i < k_segments; i++)
  {
    sfml_vertices[i].position = sfml_center + radius*sf::Vector2f(cosf(theta), sinf(theta));
    sfml_vertices[i].color = sfml_color;
    theta += k_increment;
  }
  sfml_vertices[k_segments].position = sfml_vertices[0].position;
  sfml_vertices[k_segments].color = sfml_vertices[0].color;
  window_.draw(sfml_vertices);
}

float smoothstep(float x) { return x * x * (3 - 2 * x); }

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	constexpr float k_segments = 16.0f;
	constexpr float k_increment = 2.0f * b2_pi / k_segments;
  float theta = 0.0f;
  sf::VertexArray sfml_vertices(sf::TrianglesFan, k_segments+2);
  const sf::Color& sfml_color = ConvertColor(color);
  sf::Vector2f sfml_center = ConvertVector(center);
  sfml_vertices[0].position = sfml_center;
  sfml_vertices[0].color = sfml_color;
  for (auto i = 1; i < k_segments + 2; i++)
  {
    sfml_vertices[i].position = sfml_center + radius*sf::Vector2f(cosf(theta), sinf(theta));
    sfml_vertices[i].color = sfml_color;
    theta += k_increment;
  }
  window_.draw(sfml_vertices);
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
  sf::VertexArray line(sf::Lines, 2);
  line[0].position = ConvertVector(p1);
  line[1].position = ConvertVector(p2);
  line[0].color = sf::Color(ConvertColor(color));
  line[1].color = sf::Color(ConvertColor(color));
  window_.draw(line);
}

void DebugDraw::DrawTransform(const b2Transform& xf)
{
  constexpr float k_axisScale = 1.4f;
  const sf::Vector2f& p1 = ConvertVector(xf.p);
  sf::Vector2f p2;

  sf::VertexArray line(sf::Lines, 4);
  line[0] = p1;
  line[0].color = sf::Color(255, 0, 0);
  p2 = p1 + k_axisScale * ConvertVector(xf.q.GetXAxis());
  line[1] = p2;
  line[1].color = sf::Color(255, 0, 0);

  line[2] = p1;
  line[2].color = sf::Color(0, 255, 0);
  p2 = p1 + k_axisScale * ConvertVector(xf.q.GetYAxis());
  line[3] = p2;
  line[3].color = sf::Color(0, 255, 0);

  window_.draw(line);
}

void DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
  this->DrawCircle(p, size, color);
}

void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
  sf::VertexArray quad(sf::Quads, 4);
  quad[0] = sf::Vector2f(aabb->lowerBound.x, -aabb->lowerBound.y);
  quad[1] = sf::Vector2f(aabb->upperBound.x, -aabb->lowerBound.y);
  quad[2] = sf::Vector2f(aabb->upperBound.x, -aabb->upperBound.y);
  quad[3] = sf::Vector2f(aabb->lowerBound.x, -aabb->upperBound.y);
  for (auto i = 0; i < 4; i++)
    quad[i].color = ConvertColor(c);
  window_.draw(quad);
}


void DebugDraw::set_mouse_coordinates(const float& x, const float& y)
{
  mouse_coordinates_.x = x;
  mouse_coordinates_.y = y;
}

void DebugDraw::DrawMouseCoordinates()
{
  const sf::View& view = window_.getView();
  const sf::Vector2f& viewCenter = view.getCenter();
  const sf::Vector2f& viewSize = view.getSize();
  sf::Vector2f textPosition(viewCenter.x - viewSize.x/2,
                            viewCenter.y - viewSize.y/2);
  const sf::Vector2u& window_size = window_.getSize();
  const sf::Vector2f text_scale(viewSize.x/window_size.x, viewSize.y/window_size.y);
  mouse_coordinate_box_.UpdateText(mouse_coordinates_, text_scale);
  window_.draw(mouse_coordinate_box_);
}

void DebugDraw::DrawDebugLines()
{
  window_.draw(debuglines_);
}

DebugLines& DebugDraw::get_debuglines()
{
  return debuglines_;
}

void DebugDraw::DrawParticles(const b2Vec2 *centers, float32 radius, const b2ParticleColor *colors, int32 count)
{
  for (auto i = 0; i < count; i++)
  {
    this->DrawSolidCircle(centers[i], radius, centers[i], b2Color(colors[i].r, colors[i].g, colors[i].b));

    /* points[i].position.x = centers[i].x; */
    /* points[i].position.y = -centers[i].y; */
    /* points[i].color.r = 255*(1 - colors[i].r); */
    /* points[i].color.g = 255*(1 - colors[i].g); */
    /* points[i].color.b = 255*(1 - colors[i].b); */

  }
  /* window_.draw(points); */
}


// not implemented

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

