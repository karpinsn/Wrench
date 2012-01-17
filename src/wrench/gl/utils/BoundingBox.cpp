#include "BoundingBox.h"

wrench::gl::utils::BoundingBox::BoundingBox()
{
  //  Set min to the maximum posible value and max to the miniumum possible value
  min = glm::vec3(FLT_MAX);
  max = glm::vec3(-FLT_MAX);
  center = glm::vec3(0.0f);
}

float wrench::gl::utils::BoundingBox::getMaximumDimension()
{
  return glm::max(glm::max(max.x, max.y), max.z);
}

float wrench::gl::utils::BoundingBox::getMinimumDimension()
{
  return glm::min(glm::min(min.x, min.y), min.z);
}