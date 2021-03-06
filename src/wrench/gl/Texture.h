/*
 Filename:		Texture.h
 Author:		Nikolaus Karpinsky
 Date Created:	09/20/10
 Last Edited:	12/07/10
 
 Revision Log:
 09/20/10 - Nik Karpinsky - Original creation.
 10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
 12/07/10 - Nik Karpinsky - Added methods for transfer to and from using DMA
 */

#ifndef _WRENCH_GL_TEXTURE_H_
#define _WRENCH_GL_TEXTURE_H_

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

#include "../config-wrench.h"

#include "Converter.h"
#include "OGLStatus.h"
#include "FBO.h"

#include <cv.h>

#ifdef USE_REACTOR
#include <reactor/MediaFrame.h>
#endif

#ifdef USE_VRJ
#include <vrj/Draw/OpenGL/ContextData.h>
#endif

namespace wrench
{
namespace gl
{
class FBO;

class Texture
{
private:
  GLuint  m_width;			//	Width of the texture
  GLuint  m_height;			//	Height of the texture

  int     m_dataSize;		//	Size of the data. i.e. sizeof(m_dataType...)

  //	Consult glTexImage2D for what these variables do
  GLint   m_internalFormat;	//	Internal format of the texture to render to
  GLenum  m_format;			//	Format of the texture
  GLenum  m_dataType;		//	Data type of the texture
  FBO    *m_fbo;                //  Used to read back from the texture if needed (lazy init)


#ifdef USE_VRJ
  vrj::opengl::ContextData<GLuint> vrjTextureHandle;
  vrj::opengl::ContextData<GLuint> vrjPBOHandle;
#define m_textureId (*vrjTextureHandle)
#define m_PBOId     (*vrjPBOHandle)
#else
  GLuint m_textureId;		// Texture identifier
  GLuint m_PBOId;			// PBO identifier. Used for fast DMA transfers
#endif

public:
  Texture(void);
  ~Texture();

  bool	init  (const GLuint width, const GLuint height, GLint internalFormat = GL_RGBA32F_ARB, GLenum format = GL_RGBA, GLenum dataType = GL_FLOAT);
  bool  reinit(const GLuint width, const GLuint height, GLint internalFormat = GL_RGBA32F_ARB, GLenum format = GL_RGBA, GLenum dataType = GL_FLOAT);
  void  bind() const;
  void  bind(GLenum texture) const;
  void	unbind();

  const	GLuint	getTextureId(void)      const;
  const	GLenum	getTextureTarget(void)	const;
  const	int     getChannelCount(void)	const;
  const	GLuint	getWidth(void)          const;
  const	GLuint	getHeight(void)         const;
  const	GLuint	getFormat(void)         const;
  const	GLuint	getDataType(void)       const;

  bool		transferFromTexture( IplImage* image );
  cv::Mat	transferFromAsCvMat( void );

  bool		transferToTexture( const cv::Mat image );
  bool		transferToTexture( const IplImage* image );
  
#ifdef USE_REACTOR
  bool	transferToTexture(reactor::MediaFrame& frame);
#endif

private:
  bool _checkImageCompatibility(const cv::Mat image) const;
  bool _checkImageCompatibility(const IplImage* image) const;

#ifdef USE_REACTOR
  bool _checkImageCompatibility(const reactor::MediaFrame& frame) const;
#endif
};
}
}

#endif	// _WRENCH_GL_TEXTURE_H_
