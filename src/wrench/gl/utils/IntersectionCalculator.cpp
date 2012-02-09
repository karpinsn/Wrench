#include "IntersectionCalculator.h"

bool wrench::gl::utils::IntersectionCalculator::planeLineIntersection(const glm::vec4& plane, 
																	  const glm::vec3& lineStart, 
																	  const glm::vec3& lineEnd, 
																			glm::vec3& intersectionPoint)
{
  glm::vec3 delta = lineEnd - lineStart;
  
  float denominator = (plane.x*delta.x + plane.y*delta.y + plane.z*delta.z);
  if (! denominator) return false;

  float C = (plane.x*lineStart.x + plane.y*lineStart.y + plane.z*lineStart.z + plane.w) / denominator;
  intersectionPoint = lineStart - delta * glm::vec3(C);

  return true;
}

bool wrench::gl::utils::IntersectionCalculator::sphereLineIntersection(const glm::vec3& sphereCenter,
						 											   const float		sphereRadius,
						 											   const glm::vec3& lineStart,
																	   const glm::vec3& lineEnd,
																			 glm::vec3& frontIntersection,
																			 glm::vec3& backIntersection)
{
  glm::vec3 lineDirection = lineEnd - lineStart;
  lineDirection = glm::normalize(lineDirection);

  glm::vec3 v = lineStart - sphereCenter;
  float B = 2.0f * glm::dot(lineDirection, v);
  float C = glm::dot(v, v) - sphereRadius * sphereRadius;
    
  float discriminant = B * B - 4.0f * C;

  if (discriminant < 0.0f) // Line and sphere don't intersect.
      return false;

  float discriminantSqroot = sqrtf(discriminant);
  float t0 = (-B - discriminantSqroot) * 0.5f;
  frontIntersection = (lineStart + lineDirection) * t0;

  float t1 = (-B + discriminantSqroot) * 0.5f;
  backIntersection = lineStart + lineDirection * t1;

  return true;
}