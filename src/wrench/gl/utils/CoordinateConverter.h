/**
 * @file
 * @author Nikolaus Karpinsky
 */

#ifndef _WRENCH_GL_UTILS_COORDINATE_CONVERTER_H_
#define _WRENCH_GL_UTILS_COORDINATE_CONVERTER_H_

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

#include <glm/glm.hpp>
#include <glm/gtx/type_ptr.hpp>

namespace wrench
{
  namespace gl
  {
	namespace utils
	{
	  class CoordinateConverter
	  {
	  public:
		const glm::vec4 world2Screen(const glm::vec4& worldCoordinates);
		const glm::vec4 screen2World(const glm::vec4& screenCoordinates);
		const glm::vec4 screen2World(const glm::mat4& modelView, const glm::vec4& screenCoordinates);
	  };
	}
  }
}

#endif // _WRENCH_GL_UTILS_COORDINATE_CONVERTER_H_