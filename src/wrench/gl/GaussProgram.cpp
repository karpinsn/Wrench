#include "GaussProgram.h"

using namespace std;

namespace wrench
{
  namespace gl
  {
	GaussProgram::GaussProgram(int KernelSize) : ShaderProgram(), m_kernelSize(KernelSize), m_sigma((float)KernelSize/3.0f)
	{ }

	GaussProgram::GaussProgram(int KernelSize, float Sigma) : m_kernelSize(KernelSize), m_sigma(Sigma)
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
		// Got this formula from wikipedia, worked in test code, needs checked for transcription errors
		coefficients[i] = (1.0f / sqrt(2.0f * 3.14159265359f * pow(m_sigma,2.0f))) * exp(-1.0f * (pow(i - ((float)m_kernelSize/2.0f),2.0f)/(2.0f*pow(m_sigma, 2.0f))));
		sum += coefficients[i];
	  }
	  for(int i = 0; i < m_kernelSize; ++i)
	  {
		coefficients[i] /= sum;
	  }

	  return coefficients;
	}

	const char* GaussProgram::_generateVertShaderSource( )
	{
	  string str;
	  str+="#version 130\n precision highp float; out vec2 fragTexCoordOffset[";
	  str+= to_string((long long)m_kernelSize);
	  str+= "];\n void main(void)\n {\n";
	  for(int i = 0; i < m_kernelSize; i++)
	  {
		str+= "fragTexCoordOffset[";
		str+= to_string((long long) i);
		str+= "] = ";
		str+= to_string((long long)(i -(m_kernelSize/2)));
		str+=  ";\n";
	  }
	  str += "}";
	  return str.c_str();
	}

	const char* GaussProgram::_generateFragShaderSource( )
	{
	  auto coefficients = _GetGaussCoeffs( );

	  string str;
	  str += "#version 130\n precision highp float;\n uniform sampler2D image; uniform bool h_or_V;\n in vec2 fragTexCoordOffset[";
	  str += to_string((long long)m_kernelSize);
	  str += "];\n out vec4 filteredImage; \n void main(void) \n { \n";
	  str += "if(h_or_v == true)//true = horizontal\n {\n";
	  for(int i = 0; i < m_kernelSize; i ++)
	  {
		//line looks like this : 
		//filteredImage += texture2D(image, fragTexCoordOffset[i]) * (*(weightBuffer + i));\n
		//i and *(weightBuffer + i) get hard-coded as values in the shader code
		str += "filteredImage += texture2D(image, vec2(gl_FragCoord.x + glfragTexCoordOffset[";
		str += to_string((long long)i);
		str += "], gl_FragCoord.y))*";
		str += to_string((long long) coefficients[i]);
		str += "; \n";
	  }
	  for(int i = 0; i < m_kernelSize; i ++)
	  {
		//line looks like this : 
		//filteredImage += texture2D(image, fragTexCoordOffset[i]) * (*(weightBuffer + i));\n
		//i and *(weightBuffer + i) get hard-coded as values in the shader code
		str += "filteredImage += texture2D(image, vec2(gl_FragCoord.x, gl_FragCoord.y + glfragTexCoordOffset[";
		str += to_string((long long)i);
		str += "]))*";
		str += to_string((long long) coefficients[i]);
		str += "; \n";
	  }
	  str += "} \n else\n{\n";
	  str+=" }\n}";
	  return str.c_str();
	}

	void GaussProgram::init()
	{
	  if(GLEW_VERSION_2_0)
	  {
		//Set up a handle to our shader program
		m_shaderID = glCreateProgram();

		//Begin Frag Shader Generation
		m_fragID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* shaderSource = _generateFragShaderSource( );
		
		glShaderSource(m_fragID, 1, &shaderSource, 0);
		glCompileShader(m_fragID);
		int status;
		glGetShaderiv(m_fragID, GL_COMPILE_STATUS, &status);
		
		if(status != GL_TRUE)
		  { Logger::logError("Frag Shader did not compile"); }

		//End Frag Shader Generation
		//Begin Vertex Shader Generation
		m_vertID = glCreateShader(GL_VERTEX_SHADER);
		shaderSource = _generateVertShaderSource( );
		glShaderSource(m_vertID, 1, &shaderSource, 0);
		glGetShaderiv(m_vertID, GL_COMPILE_STATUS, &status);
		
		if(status != GL_TRUE)
		  { Logger::logError("Vert Shader did not compile"); }
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
  }
}
