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

void wrench::gl::utils::Arcball::mousePressEvent(const GLint mouseX, const GLint mouseY)
{ 
  glm::vec4 worldCoord = m_converter.screen2World(glm::vec4(mouseX, mouseY, 0.0f, 1.0));

  m_startPoint = worldCoord;

  // Map the point to the sphere
  m_startVector = mapToSphere(glm::vec2(worldCoord.x, worldCoord.y));
  m_startQuat = m_currentQuat;
}

void wrench::gl::utils::Arcball::mouseDragEvent(const GLint mouseX, const GLint mouseY)
{
  glm::vec4 worldCoord = m_converter.screen2World(glm::vec4(mouseX, mouseY, 0.0f, 1.0));

  m_endPoint = worldCoord;

  // Map the point to the sphere
  m_endVector = mapToSphere(glm::vec2(worldCoord.x, worldCoord.y));

  m_currentQuat = glm::cross(m_startQuat, glm::quat(glm::dot(m_startVector, m_endVector), glm::cross(m_startVector, m_endVector)));
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

glm::vec3 wrench::gl::utils::Arcball::mapToSphere(const glm::vec2& point) const
{
  glm::vec3 mappedVector(0.0f);
  
  mappedVector.x  = (point.x - m_center.x) / m_radius;
  mappedVector.y  = (point.y - m_center.y) / m_radius;

  //	Compute the length of the vector to see if it is inside or outside the circle
  GLfloat length = glm::length(mappedVector);

  if (length > 1.0f)	//	It's on the outside
  {
	//	Normalize the vector
	mappedVector = glm::normalize(mappedVector);
  }
  else				//	It's on the inside
  {
	//	Return a vector to a point mapped inside the sphere sqrt(radius squared - length)
	mappedVector.z = glm::sqrt(1.0f - length);
  }

  return mappedVector;
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

    glPushMatrix();
  glLoadIdentity();

  glDisable(GL_LIGHTING);
  glPointSize(2.0f);
  glBegin(GL_POINTS);
  glColor3f(0.0f, 1.0f, 0.0f);


  glVertex3f(m_startPoint.x, m_startPoint.y, m_startPoint.z);


    glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(m_endPoint.x, m_endPoint.y, m_endPoint.z);
  glEnd();

    glPopMatrix();

  glEnable(GL_LIGHTING);

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}