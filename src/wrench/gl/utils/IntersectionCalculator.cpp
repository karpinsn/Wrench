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
  frontIntersection = lineStart + lineDirection * t0;

  float t1 = (-B + discriminantSqroot) * 0.5f;
  backIntersection = lineStart + lineDirection * t1;

  return true;
}

bool wrench::gl::utils::IntersectionCalculator::sphereRayIntersection(	const glm::vec3& sphereCenter,
																		const float		 sphereRadius,
																		const glm::vec3& rayOrigin,
																		const glm::vec3& rayDirection,
																			  glm::vec3& intersection)
{
  //  Calculate A, B, and C coefficents of quadratic formula
  const float A = glm::dot(rayDirection, rayDirection);
  const float B = 2.0f * glm::dot((rayOrigin - sphereCenter), rayDirection);
  const float C = glm::dot((rayOrigin - sphereCenter), (rayOrigin - sphereCenter)) - (sphereRadius * sphereRadius);

  //  Find discriminat
  float disc = B * B - 4.0f * A * C;

  if(disc < 0.0f) //  Ray and sphere don't intersect
	return false;

  float discSqrt = sqrtf(disc);
  float q;
  if (B < 0)
  {
	q = (-B - discSqrt) / 2.0f;
  }
  else
  {
	q = (-B + discSqrt) / 2.0f;
  }

  //  Compute t0 and t1 ... Used for numeric stability
  float t0 = q / A;
  float t1 = C / q;

  // if t1 is less than zero, the object is in the ray's negative direction
  // and consequently the ray misses the sphere
  if (t1 < 0)
      return false;

  // if t0 is less than zero, the intersection point is at t1
  float t = t0 < 0 ? t1 : t0;

  intersection = rayOrigin + rayDirection * t;
  return true;
}