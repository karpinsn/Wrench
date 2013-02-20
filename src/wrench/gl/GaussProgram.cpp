#include "GaussProgram.h"

using namespace std;

namespace wrench
{
  namespace gl
  {
	GaussProgram::GaussProgram(int KernelSize) : 
	  ShaderProgram(), m_kernelSize(KernelSize), m_sigma((float)KernelSize/3.0f), m_horizontalFilter(false)
	{ }

	GaussProgram::GaussProgram(int KernelSize, float Sigma) : 
	  ShaderProgram(), m_kernelSize(KernelSize), m_sigma(Sigma), m_horizontalFilter(false)
	{ }

	GaussProgram::~GaussProgram( )
	{
	  // Delete our shaders
	  glDeleteShader( m_fragID );
	  glDeleteShader( m_vertID );

	  // Now delete the program
	  glDeleteProgram( m_shaderID );
	}

	unique_ptr<float[]> GaussProgram::_GetGaussCoeffs( )
	{
	  auto coefficients = unique_ptr<float[]>( new float[m_kernelSize]);

	  float sum = 0;
	  for (int i = 0; i < m_kernelSize; ++i)
	  {
		int offset = i - ( ( m_kernelSize - 1 ) / 2 );
		coefficients[i] = exp(-1.0f * (float)(offset * offset) / ( 2.0f * m_sigma * m_sigma)); 
		sum += coefficients[i];
	  }
	  //  Quick normalization
	  for(int i = 0; i < m_kernelSize; ++i)
		{ coefficients[i] /= sum; }

	  return coefficients;
	}

	const int GaussProgram::_attachVertShader( )
	{
	  string source;
	  int status;

	  source += "#version 130\n";
	  source += "precision highp float;\n\n";
	  source += "uniform float width;\n";
	  source += "uniform float height;\n";
	  source += "uniform bool horizontalFilter;\n\n";
	  source += "in vec3 vert;\n";
	  source += "in vec2 vertTexCoord;\n";
	  source += "out vec2 fragTexCoordOffset[" + to_string((long long)m_kernelSize) + "];\n\n";
	  source += "float step_w = 1.0 / width * float(horizontalFilter);\n";
	  source += "float step_h = 1.0 / height * float(!horizontalFilter);\n\n";
	  source += "void main(void)\n";
	  source += "{\n";
	  for(int i = 0; i < m_kernelSize; i++)
	  {
		// Currently VC++10 does not have all the to_string overloads in. Use long double to circumvent this
		long double offset = i - ( ( m_kernelSize - 1 ) / 2 );

		source += "\tfragTexCoordOffset[" + to_string((long long) i) + "] = ";
		source += "vertTexCoord + vec2(" + ::to_string(offset) + " * step_w," + ::to_string(offset) + " * step_h);\n";
	  }
	  source += "\tgl_Position = vec4(vert, 1.0);\n";
	  source += "}";
	  
	  const char* shaderSource = source.c_str( );

	  m_vertID = glCreateShader(GL_VERTEX_SHADER);
	  glShaderSource(m_vertID, 1, &shaderSource, 0);
	  glCompileShader(m_vertID);
	  glGetShaderiv(m_vertID, GL_COMPILE_STATUS, &status);
	  glAttachShader(m_shaderID, m_vertID);
	  return status;
	}

	const int GaussProgram::_attachFragShader( )
	{
	  int status;
	  string source;
	  auto coefficients = _GetGaussCoeffs( );
	  
	  source += "#version 130\n";
	  source += "precision highp float;\n\n";
	  source += "uniform sampler2D image;\n\n";
	  source += "in vec2 fragTexCoordOffset[" + to_string((long long)m_kernelSize) + "];\n";
	  source += "out vec4 filteredImage;\n\n";
	  source += "void main(void)\n";
	  source += "{ \n";
	  source += "\tfilteredImage = vec4(0.0);\n";
	  for(int i = 0; i < m_kernelSize; i ++)
	  {
		source += "\tfilteredImage += texture(image, ";
		source += "fragTexCoordOffset[" + to_string((long long)i) + "]) * ";
		source += to_string((long double) coefficients[i]) + ";\n";
	  }
	  source +="}\n";

	  const char* shaderSource = source.c_str( );

	  m_fragID = glCreateShader(GL_FRAGMENT_SHADER);
	  glShaderSource(m_fragID, 1, &shaderSource, 0);
	  glCompileShader(m_fragID);
	  glGetShaderiv(m_fragID, GL_COMPILE_STATUS, &status);
	  glAttachShader(m_shaderID, m_fragID);

	  return status;
	}

	void GaussProgram::setImageDimensions(int width, int height)
	{
	  uniform("width", (float)width);
	  uniform("height", (float)height);
	}

	void GaussProgram::init()
	{
	  if(GLEW_VERSION_2_0)
	  {
		//Set up a handle to our shader program
		m_shaderID = glCreateProgram();

		// Create and attach the fragment shader
		if(_attachFragShader( ) != GL_TRUE)
		  { Logger::logError("Frag Shader did not compile"); }

		// Create and attach the vert shader
		
		if(_attachVertShader( ) != GL_TRUE)
		  { Logger::logError("Vert Shader did not compile"); }

		//	Finally link and set the filter direction
		link();
		uniform("horizontalFilter", m_horizontalFilter);
	  }
	  else
	  {
		Logger::logError("GL version not sufficient");
	  }
	}

	void GaussProgram::attachShader(Shader *shader)
	{
	  Logger::logError("Attaching custom shaders not supported by Gauss Program");
	}

	void GaussProgram::flipFilter( void )
	{
	  m_horizontalFilter = !m_horizontalFilter;
	  uniform("horizontalFilter", m_horizontalFilter);
	}
  }
}
