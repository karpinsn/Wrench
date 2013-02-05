#include "CoordinateConverter.h"

const glm::vec4 wrench::gl::utils::CoordinateConverter::world2Screen(const glm::vec4& worldCoordinates)
{
  glm::mat4 modelView;
  glm::mat4 projection;

  glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelView));
  glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
  
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glm::vec4 cameraCoord = projection * modelView * worldCoordinates;
  glm::vec4 screenCoord = cameraCoord;
  screenCoord.x = (cameraCoord.x + 1.0f) / 2.0f * (float)viewport[2] + (float)viewport[0];
  screenCoord.y = (1.0f - cameraCoord.y) / 2.0f * (float)viewport[3] + (float)viewport[1];

  return screenCoord;
}

const glm::vec4 wrench::gl::utils::CoordinateConverter::screen2World(const glm::vec4& screenCoordinates)
{
  glm::mat4 modelView;
  glm::mat4 projection;

  glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(modelView));
  glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
  
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glm::vec4 cameraXYZ;
  cameraXYZ.x = (screenCoordinates.x - (float)viewport[0]) / (float)viewport[2] * 2.0f - 1.0f;
  cameraXYZ.y = (screenCoordinates.y - (float)viewport[1]) / (float)viewport[3] * 2.0f - 1.0f;
  cameraXYZ.z = screenCoordinates.z;
  cameraXYZ.w = 1.0f;

  glm::mat4 cameraInverse = glm::inverse(modelView * projection);
  return cameraInverse * cameraXYZ;
}

const glm::vec4 wrench::gl::utils::CoordinateConverter::screen2World(const glm::mat4& modelView, const glm::vec4& screenCoordinates)
{
  glm::mat4 projection;

  glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(projection));
  
  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glm::vec4 cameraXYZ;
  cameraXYZ.x = (screenCoordinates.x - (float)viewport[0]) / (float)viewport[2] * 2.0f - 1.0f;
  cameraXYZ.y = (screenCoordinates.y - (float)viewport[1]) / (float)viewport[3] * 2.0f - 1.0f;
  cameraXYZ.z = screenCoordinates.z;
  cameraXYZ.w = 1.0f;

  return glm::inverse(modelView) * glm::inverse(projection) * cameraXYZ;
}