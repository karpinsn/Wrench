#include "IntersectionCalculator.h"

bool wrench::gl::utils::IntersectionCalculator::planeLineIntersection(const glm::vec4& plane, 
																	  const glm::vec4& lineStart, 
																	  const glm::vec4& lineEnd, 
																			glm::vec4& intersectionPoint)
{
  glm::vec4 delta = lineEnd - lineStart;
  
  float denominator = (plane.x*delta.x + plane.y*delta.y + plane.z*delta.z);
  if (! denominator) return false;

  float C = (plane.x*lineStart.x + plane.y*lineStart.y + plane.z*lineStart.z + plane.w) / denominator;
  intersectionPoint = lineStart - delta * glm::vec4(C);

  return true;
}

bool wrench::gl::utils::IntersectionCalculator::sphereLineIntersection(const glm::vec4& sphereCenter,
						 											   const float		sphereRadius,
						 											   const glm::vec4& lineStart,
																	   const glm::vec4& lineEnd,
																			 glm::vec4& frontIntersection,
																			 glm::vec4& backIntersection)
{
  glm::vec4 lineDirection = lineEnd - lineStart;
  lineDirection = glm::normalize(lineDirection);

  glm::vec4 v = lineStart - sphereCenter;
  float B = 2.0f * glm::dot(lineDirection, v);
  float C = glm::dot(v, v) - sphereRadius * sphereRadius;
    
  float discriminant = B * B - 4.0f * C;

  if (discriminant < 0.0f) // Line and sphere don't intersect.
      return false;

  float discriminantSqroot = sqrtf(discriminant);
  float t0 = (-B - discriminantSqroot) * 0.5f;
  frontIntersection = lineStart + lineDirection * t0;

  float t1 = (-B + discriminantSqroot) * 0.5f;
  backIntersection = lineStart + lineDirection * t1;

  return true;
}

bool wrench::gl::utils::IntersectionCalculator::sphereRayIntersection(	const glm::vec4& sphereCenter,
																		const float		 sphereRadius,
																		const glm::vec4& rayOrigin,
																		const glm::vec4& rayDirection,
																			  glm::vec4& intersection)
{
  //  Calculate B, and C coefficents of quadratic formula
  const float B = 2.0f * glm::dot(rayOrigin - sphereCenter, rayDirection);
  const float C = glm::dot(rayOrigin - sphereCenter, rayOrigin - sphereCenter) - (sphereRadius * sphereRadius);

  //  Find discriminat
  float disc = B * B - 4.0f * C;

  if(disc < 0.0f) //  Ray and sphere don't intersect
	return false;

  float discSqrt = sqrtf(disc);
  float t = (-B - discSqrt) * .5f;
  intersection = rayOrigin + rayDirection * t;
  
  return true;
}