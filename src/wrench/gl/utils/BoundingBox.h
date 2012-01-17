/**
 Filename:	BoundingBox.h
 Author:	Nikolaus Karpinsky
 Date Created:	01/18/12
 Last Edited:	01/18/12
 
 Revision Log:
 01/30/11 - Nik Karpinsky - Original creation.
 */
#ifndef _WRENCH_BOUNDING_BOX_H_
#define _WRENCH_BOUNDING_BOX_H_

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

//	GLM includes
#include <glm/glm.hpp>

#include <float.h>

namespace wrench
{
  namespace gl
  {
    namespace utils
    {
      class BoundingBox
      {
      public:
		glm::vec3 min;
		glm::vec3 max;
		glm::vec3 center;
      
	  public:
		BoundingBox();

		float getMaximumDimension();
		float getMinimumDimension();
      };
    }
  }
}

#endif	// _WRENCH_BOUNDING_BOX_H_
