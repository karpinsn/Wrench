/*
 Filename:		GaussianFragShader.h
 Author:		Morgan Hoke, based on Shader.h by Nik Karpinsky
 Date Created:	09/20/10
 Last Edited:	09/20/10
 
 Revision Log:
 10/2/2012  - created
 */

#ifndef _WRENCH_GL_GAUSS_FRAG_SHADER_H_
#define _WRENCH_GL_GAUSS_FRAG_SHADER_H_

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
#include "Shader.h"

#include "OGLStatus.h"

#ifdef USE_VRJ
    #include <vrj/Draw/OpenGL/ContextData.h>
#endif

using namespace std;

namespace wrench
{
	namespace gl 
	{
        class GaussFragShader : public Shader
		{
		private:
			#ifdef USE_VRJ
                vrj::opengl::ContextData<GLuint> vrjShaderID;
                #define m_shaderID (*vrjShaderID)
			#else
                GLuint m_shaderID;
			#endif
             GaussFragShader(); //Killing the default constructor since we can't instantiate from a file
		public:
             GaussFragShader(int KernelSize, float Sigma);
             GaussFragShader(int KernelSize);
            ~GaussFragShader();
             virtual bool init(GLenum shaderType, const string &filename);
        private:
            bool _validateShader(GLuint shader, const string &filename);
            bool generateAndCompileShader(int KernelSize, float Sigma);
		};
	}
}

#endif	// _WRENCH_GL_SHADER_H_
