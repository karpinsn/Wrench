#include "Arcball.h"                                   

wrench::gl::utils::Arcball::Arcball()
{	
  // Clear initial values	
  m_center		= glm::vec4(glm::vec3(0.0f), 1.0f);
  m_radius		= 1.0f;

  m_startVector = glm::vec4(glm::vec3(0.0f), 0.0f);
  m_endVector	= glm::vec4(glm::vec3(0.0f), 0.0f);

  m_startQuat	= glm::quat();
  m_currentQuat = glm::quat();

  m_isRotating = false;
}

void wrench::gl::utils::Arcball::init(float centerX, float centerY, float centerZ, float radius)
{
  //  Set values
  m_center.x  = centerX;
  m_center.y  = centerY;
  m_center.z  = centerZ;
  m_center.w  = 1.0f;
  m_radius	  = radius;

  m_startVector = glm::vec4(glm::vec3(0.0f), 0.0f);
  m_endVector	= glm::vec4(glm::vec3(0.0f), 0.0f);
}

void wrench::gl::utils::Arcball::mousePressEvent(const glm::mat4 modelView, const GLint mouseX, const GLint mouseY)
{ 
  // Map the point
  m_startVector = mapPoint(modelView, glm::vec2(mouseX, mouseY), m_isRotating) - m_center;
  m_startQuat = m_currentQuat;
}

void wrench::gl::utils::Arcball::mouseDragEvent(const glm::mat4 modelView, const GLint mouseX, const GLint mouseY)
{
  if(m_isRotating)	// Only map the point if we have a start point
  {
	// Map the point
	bool intersected = false;
	m_endVector = mapPoint(modelView, glm::vec2(mouseX, mouseY), intersected) - m_center;

	glm::vec3 axis = glm::cross(glm::vec3(m_startVector), glm::vec3(m_endVector));
	float angle = glm::dot(m_startVector, m_endVector) / (glm::length(m_startVector) * glm::length(m_endVector));
	angle /= 2.0f;

	m_currentQuat = glm::cross(glm::normalize(glm::quat(angle, axis)), m_startQuat);
  }
}

glm::mat4 wrench::gl::utils::Arcball::getTransform(void)
{
  return glm::translate(glm::mat4(1.0), glm::vec3(m_center)) * glm::mat4_cast(m_currentQuat) * glm::translate(glm::mat4(1.0), -glm::vec3(m_center));
}

void wrench::gl::utils::Arcball::applyTransform(void)
{
  glMultMatrixf(glm::value_ptr(getTransform()));
}


glm::vec4 wrench::gl::utils::Arcball::mapPoint(const glm::mat4 modelView, const glm::vec2& point, bool& isIntersected)
{ 
  glm::vec4 worldNear = m_converter.screen2World(modelView, glm::vec4(point.x, point.y, 0.0f, 1.0));
  glm::vec4 worldFar  = m_converter.screen2World(modelView, glm::vec4(point.x, point.y, 1.0f, 1.0)); 

  glm::vec4 rayDirection = worldFar - worldNear;
  rayDirection = glm::normalize(rayDirection);

  glm::vec4 intersection(glm::vec3(0.0f), 1.0f);
  isIntersected = m_intersectionCalc.sphereRayIntersection(m_center, m_radius, worldNear, rayDirection, intersection);

  if(!isIntersected)
  {
	//	Grab the closest point
	//intersection = (rayDirection) * glm::dot(rayDirection, m_center - worldNear);
	intersection = worldNear + rayDirection * glm::dot(m_center - worldNear, rayDirection);
  }

  intersection.w = 1.0f;

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
  applyTransform();
  glTranslatef(m_center.x, m_center.y, m_center.z);
  gluSphere(quadric, m_radius, 32, 32);
  glPopMatrix();

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}