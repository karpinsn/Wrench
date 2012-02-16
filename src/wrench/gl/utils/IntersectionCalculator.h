#ifndef _WRENCH_GL_UTILS_INTERSECTION_CALCULATOR_H_
#define _WRENCH_GL_UTILS_INTERSECTION_CALCULATOR_H_

#include <glm/glm.hpp>

namespace wrench
{
  namespace gl
  {
	namespace utils
	{
	  class IntersectionCalculator
	  {
	  public:
		bool planeLineIntersection(	const glm::vec4& plane, 
									const glm::vec4& lineStart, 
									const glm::vec4& lineEnd, 
										  glm::vec4& intersectionPoint);

		bool sphereLineIntersection(const glm::vec4& sphereCenter,
									const float		 sphereRadius,
									const glm::vec4& lineStart,
									const glm::vec4& lineEnd,
										  glm::vec4& frontIntersection,
										  glm::vec4& backIntersection);

		bool sphereRayIntersection(	const glm::vec4& sphereCenter,
									const float		 sphereRadius,
									const glm::vec4& rayOrigin,
									const glm::vec4& rayDirection,
										  glm::vec4& intersection);
	  };
	}
  }
}

#endif