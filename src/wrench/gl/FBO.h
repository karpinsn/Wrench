/*
 Filename:		FBO.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	10/27/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 12/09/10 - Nik Karpinsky - Moved file into the Wrench library
 */

#ifndef _WRENCH_GL_FBO_H_
#define _WRENCH_GL_FBO_H_

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

#include "../Logger.h"
#include "Texture.h"
#include "OGLStatus.h"
#include "VAO.h"
#include "VBO.h"

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

namespace wrench 
{
  namespace gl
  {
	class Texture;

    class FBO
    {
    private:
#ifdef USE_VRJ
      vrj::opengl::ContextData<GLuint> vrjFBOHandle;
      vrj::opengl::ContextData<GLuint> vrjRBOHandle;
      vrj::opengl::ContextData<GLuint> vrjVAOHandle;
      vrj::opengl::ContextData<GLuint[2]> vrjVBOHandle;
#define m_framebuffer   (*vrjFBOHandle)
#define m_rbo           (*vrjRBOHandle)
#define m_vaoId         (*vrjVAOHandle)
#define m_vboId         (*vrjVBOHandle)
#else
      GLuint m_framebuffer;
      GLuint m_rbo;
      GLuint m_vaoId;
      GLuint m_vboId[2];
#endif

      VAO m_screen;
      VBO m_vertex;
      VBO m_texCoords;

      int m_width;
      int m_height;

		public:
      FBO(void);
      ~FBO();

      bool init(int width, int height);
      bool reinit(int width, int height);

      void bind(void);
      void unbind(void);
      void bindDrawBuffer(GLenum attachmentPoint);
	  void bindDrawBuffers( int numberOfBuffers, const GLenum *buffers );
      void setTextureAttachPoint(const Texture &texture, GLenum attachmentPoint);
	  void resetDepthBuffer( );
      void process(void);

		private:
      void _initFBO(void);
      void _cacheQuad(void);
    };
  }
}

#endif	// _WRENCH_GL_FBO_H_
