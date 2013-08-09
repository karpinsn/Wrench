/*
Filename:	ShaderProgram.h
Author:	Nikolaus Karpinsky
Date Created:	09/20/10
Last Edited:	09/20/10

Revision Log:
09/20/10 - Nik Karpinsky - Original creation.
10/27/10 - Nik Karpinsky - Allows for VRJ context specific data
*/

#ifndef _WRENCH_GL_SHADER_PROGRAM_H_
#define _WRENCH_GL_SHADER_PROGRAM_H_

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

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

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
	class ShaderProgram
	{
	protected:
#ifdef USE_VRJ
	  vrj::opengl::ContextData<GLuint> vrjShaderID;
#define m_shaderID (*vrjShaderID)
#else
	  GLuint m_shaderID;
#endif

	  vector<Shader*> m_shaders;

	public:
	  ShaderProgram();
	  virtual ~ShaderProgram();
	  virtual void init();
	  virtual void attachShader(Shader *shader);
	  bool link();

	  void bind();
	  void unbind();

	  virtual void uniform(const string name, const int data);
	  virtual void uniform(const string name, const float data);
	  virtual void uniform(const string name, const float *data, int count);
	  virtual void uniform(const string name, const glm::vec3 data);
	  virtual void uniform(const string name, const glm::vec4 data);
	  virtual void uniform(const string name, const glm::mat3 data);
	  virtual void uniform(const string name, const glm::mat4 data);
	  virtual void uniform(const string name, const glm::mat4x3 data);
	  virtual void uniformMat4(const string name, GLboolean transpose, const GLfloat *value);
	  virtual void bindAttributeLocation(const string name, const GLuint index);
	  virtual void bindOutputLocation(const string name, const GLuint index);

	private:
	  bool _validateProgram(GLuint program);
	};
  }
}

#endif	// _WRENCH_GL_SHADER_PROGRAM_H_
