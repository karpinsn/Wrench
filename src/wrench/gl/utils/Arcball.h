/**
 *	@file
 *	@author		Nikolaus Karpinsky
 *
 *	Class that provides an arcball manipulator for transforming objects in an
 *	OpenGL scene.
 */

#ifndef _ARCBALL_H_
#define _ARCBALL_H_

#ifdef __APPLE__
	#include <glew.h>
	#include <OpenGL/gl.h>
#elif _WIN32
	#include <windows.h>
	#include <GL/glew.h>
	#include <GL/gl.h>
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
#endif

#include "../VBO.h"
#include "../VAO.h"
#include "../ShaderProgram.h"
#include "../Shader.h"

#include "CoordinateConverter.h"
#include "IntersectionCalculator.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define DEG_2_RAD 3.14159f / 180.0f

namespace wrench
{
	namespace gl 
	{
	  namespace utils
	  {
		class Arcball
		{	
		public:
			Arcball();
			virtual ~Arcball() {};
			void init(float centerX, float centerY, float centerZ, float radius);
			
			//Mouse down
			void mousePressEvent(const glm::mat4 modelView, const GLint mouseX, const GLint mouseY);
			
			//Mouse drag, calculate rotation
			void mouseDragEvent(const glm::mat4 modelView, const GLint mouseX, const GLint mouseY);
			
			glm::mat4 getTransform(void);
			void applyTransform(void);
			void draw(void);  //  Draws an arcball control, useful for debugging

		protected:
			glm::vec4   m_startVector;      //	Saved click vector
			glm::vec4   m_endVector;        //	Saved drag vector

			glm::vec4 m_center;
			float m_radius;

			glm::quat m_startQuat;
			glm::quat m_currentQuat;

			bool m_isRotating;

		private:
			IntersectionCalculator m_intersectionCalc;
			CoordinateConverter m_converter;

			ShaderProgram m_shader;
			VAO m_sphereManipulatorGeometry;
			VBO m_axialRingVertex;
			VBO m_axialRingColor;
			VBO m_axialRingNormals;

			inline glm::vec4 mapPoint(const glm::mat4 modelView, const glm::vec2& point, bool& isIntersected);
			void _cacheGeometry(void);
		};
	  }
	}
}

#endif	// _ARCBALL_H_

