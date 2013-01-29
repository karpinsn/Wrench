/*
 Filename:		IShader.h
 Author:		Morgan Hoke
 Date Created:	10/22/12
 Last Edited:	10/22/12
 
 Revision Log:
 10/22/12 - Morgan Hoke - Original creation
 */

#ifndef _WRENCH_GL_ISHADER_H_
#define _WRENCH_GL_ISHADER_H_

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

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include "OGLStatus.h"

#ifdef USE_VRJ
    #include <vrj/Draw/OpenGL/ContextData.h>
#endif

using namespace std;

namespace wrench
{
	namespace gl 
	{
		class IShader 
		{
		private:
			#ifdef USE_VRJ
                vrj::opengl::ContextData<GLuint> vrjShaderID;
                #define m_shaderID (*vrjShaderID)
			#else
                GLuint m_shaderID;
			#endif
		public:
			virtual bool init();
			virtual GLuint shaderID(void);
		};
	}
}

#endif	// _WRENCH_GL_SHADER_H_
