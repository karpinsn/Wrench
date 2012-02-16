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

  _cacheGeometry();
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
	m_intersectionCalc.sphereLineIntersection(m_center, m_radius, worldNear, m_center, intersection, glm::vec4(0.0f));
  }

  intersection.w = 1.0f;

  return intersection;
}

#include<GL/GLU.h>
GLUquadricObj *quadric;
bool first = true;
void wrench::gl::utils::Arcball::draw(void)
{
  glm::mat4 modelViewMatrix;
  glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelViewMatrix));

  m_shader.bind();
  {
	m_shader.uniform("modelViewMatrix", modelViewMatrix);
	m_sphereManipulatorGeometry.draw();
  }
  m_shader.unbind();
  
  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  if(first)
  {
	first = !first;
	quadric = gluNewQuadric();
  }

  glColor3f(.8f, .8f, .8f);
  glPushMatrix();
  applyTransform();
  glTranslatef(m_center.x, m_center.y, m_center.z);
  gluSphere(quadric, m_radius, 32, 32);
  glPopMatrix();

  glPushMatrix();
  applyTransform();
  glTranslatef(m_center.x, m_center.y, m_center.z);
  
  glLineWidth(2.0f);

  /*
  glPushAttrib(GL_ENABLE_BIT | GL_LIGHTING_BIT);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);
  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_LINE_LOOP);
  {
	//	Draw Z-axis Manipulator
	for(int theta = 0; theta < 360; ++theta)
	{
	  float x = m_radius * cosf(theta * DEG_2_RAD);
	  float y = m_radius * sinf(theta * DEG_2_RAD);
	  glm::vec3 point(x, y, 0.0f);

	  glNormal3fv(glm::value_ptr(glm::normalize(point)));
	  glVertex3fv(glm::value_ptr(point));
	}
  }
  glEnd();

  glPopAttrib();*/

  glPopMatrix();

  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void wrench::gl::utils::Arcball::_cacheGeometry(void)
{
  float verticies[360*3];
  float normals[360*3];

  //  Cache the Axial Ring Geometry
  for(int theta = 0; theta < 360; ++theta)
  {
	float x = m_radius * cosf(theta * DEG_2_RAD);
	float y = m_radius * sinf(theta * DEG_2_RAD);
	glm::vec3 point(x, y, 0.0f);
	verticies[theta*3] = x;
	verticies[theta*3+1] = y;
	verticies[theta*3+2] = 0.0f;
	//verticies[theta] = glm::vec3(x, y, 0.0f);
	verticies[theta*3] = glm::normalize(point).x;
	verticies[theta*3+1] = glm::normalize(point).y;
	verticies[theta*3+2] = 0.0f;
	//normals[theta] = glm::normalize(verticies[theta]);
  }

  m_sphereManipulatorGeometry.init(GL_LINE_LOOP, 360);
  m_axialRingVertex.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
  m_axialRingVertex.bufferData(360, verticies, GL_STATIC_DRAW);
  m_sphereManipulatorGeometry.addVBO(m_axialRingVertex, "vert");

  m_axialRingNormals.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
  m_axialRingNormals.bufferData(360, normals, GL_STATIC_DRAW);
  m_sphereManipulatorGeometry.addVBO(m_axialRingNormals, "vertNormal");

  m_shader.init();
  m_shader.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/RotationManipulator.vert"));
  m_shader.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/RotationManipulator.frag"));
  m_shader.bindAttributeLocation("vert", 0);
  m_shader.bindAttributeLocation("vertNormal", 1);
  m_shader.link();

  OGLStatus::logOGLErrors("wrench::gl::utils::Arcball - _cacheGeometry()");
}