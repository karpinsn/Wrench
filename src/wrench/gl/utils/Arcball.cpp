#include "Arcball.h"                                   

wrench::gl::utils::Arcball::Arcball()
{	
  // Clear initial values	
  m_center.x  = 0.0f;
  m_center.y  = 0.0f;
  m_center.z  = 0.0f;
  m_radius	  = 1.0f;

  m_startVector = glm::vec3(0.0f);
  m_endVector	= glm::vec3(0.0f);

  m_startQuat	= glm::quat();
  m_currentQuat = glm::quat();
}

void wrench::gl::utils::Arcball::init(float centerX, float centerY, float centerZ, float radius)
{
  //  Set values
  m_center.x  = centerX;
  m_center.y  = centerY;
  m_center.z  = centerZ;
  m_radius	  = radius;

  m_startVector = glm::vec3(0.0f);
  m_endVector	= glm::vec3(0.0f);
}

#include <iostream>

void wrench::gl::utils::Arcball::mousePressEvent(const GLint mouseX, const GLint mouseY)
{ 
  // Map the point
  m_startVector = mapPoint(glm::vec2(mouseX, mouseY));
  m_startQuat = m_currentQuat;
}

void wrench::gl::utils::Arcball::mouseDragEvent(const GLint mouseX, const GLint mouseY)
{
  // Map the point
  m_endVector = mapPoint(glm::vec2(mouseX, mouseY));

  m_currentQuat = glm::cross(glm::quat(glm::dot(m_startVector, m_endVector) / 2.0f, glm::cross(m_endVector, m_startVector)), m_startQuat);
  m_currentQuat = glm::normalize(m_currentQuat);
}

glm::mat4 wrench::gl::utils::Arcball::getTransform(void)
{
  return glm::translate(glm::mat4(1.0), m_center) * glm::mat4_cast(m_currentQuat) * glm::translate(glm::mat4(1.0), -m_center);
}

void wrench::gl::utils::Arcball::applyTransform(void)
{
  glMultMatrixf(glm::value_ptr(getTransform()));
}

#include <iostream>

glm::vec3 wrench::gl::utils::Arcball::mapPoint(const glm::vec2& point)
{
  glm::vec4 worldCoord = m_converter.screen2World(glm::vec4(point.x, point.y, 0.0f, 1.0));

  glm::vec3 intersection;
  bool intersected = m_intersectionCalc.sphereRayIntersection(m_center, m_radius, glm::vec3(0.0f), glm::vec3(worldCoord) - glm::vec3(0.0f), intersection);

  return intersection;
}

#include<GL/GLU.h>
GLUquadricObj *quadric;
bool first = true;
void wrench::gl::utils::Arcball::draw(void)
{
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  if(first)
  {
	first = !first;
	quadric = gluNewQuadric();
  }

  glPushMatrix();
  glTranslatef(m_center.x, m_center.y, m_center.z);
  gluSphere(quadric, m_radius, 32, 32);
  glPopMatrix();

  glEnable(GL_LIGHTING);

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}