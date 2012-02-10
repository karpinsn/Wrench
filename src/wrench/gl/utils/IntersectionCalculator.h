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
									const glm::vec3& lineStart, 
									const glm::vec3& lineEnd, 
										  glm::vec3& intersectionPoint);

		bool sphereLineIntersection(const glm::vec3& sphereCenter,
									const float		 sphereRadius,
									const glm::vec3& lineStart,
									const glm::vec3& lineEnd,
										  glm::vec3& frontIntersection,
										  glm::vec3& backIntersection);

		bool sphereRayIntersection(	const glm::vec3& sphereCenter,
									const float		 sphereRadius,
									const glm::vec3& rayOrigin,
									const glm::vec3& rayDirection,
										  glm::vec3& intersection);
	  };
	}
  }
}

#endif