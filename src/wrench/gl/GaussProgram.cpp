#include "GaussProgram.h"

using namespace std;

namespace wrench
{
	namespace gl
	{
			GaussProgram::GaussProgram(int KernelSize) : ShaderProgram()
			{
				kernelSize = KernelSize;
				sigma = (float)KernelSize/3.0f;
			}

			GaussProgram::GaussProgram(int KernelSize, float Sigma)
			{
				kernelSize = KernelSize;
				sigma = Sigma;
			}

			void GetGaussCoeffs(float * buffer, float sigma, int kernelSize)
			{
				float sum = 0;
				for (int i = 0; i < kernelSize; i++)
				{
					// Got this formula from wikipedia, worked in test code, needs checked for transcription errors
					*(buffer + i) = (1 / sqrt(2.0 * 3.14159265359 * pow(sigma,2.0f))) * exp(-1 * (pow(i - (kernelSize/2),2.0)/(2.0*pow(sigma, 2.0f))));
					sum += *(buffer + i);
				}
				for(int i = 0; i<kernelSize; i++)
				{
					*(buffer + i) /= sum;
				}
			}

			const char * generateShaderSource(float * weightBuffer, int KernelSize)
			{
				string str;
				str += "#version 130\n precision highp float;\n uniform sampler2D image;\n uniform float kernel[";
				str += to_string((long long)KernelSize);  //C++ 11 rocks, vs2010 implementation requires the int to cast to long long!
				str += "];\n in vec2 fragTexCoordOffset[";
				str += to_string((long long)KernelSize);
				str += "];\n out vec4 filteredImage; \n void main(void) \n { \n";
				for(int i = 0; i < KernelSize; i ++)
				{
					//line looks like this : 
					//filteredImage += texture2D(image, fragTexCoordOffset[i]) * (*(weightBuffer + i));\n
					//i and *(weightBuffer + i) get hard-coded as values in the shader code
					str += "filteredImage += texture2D(image, fragTexCoordOffset[";
					str += to_string((long long)i);
					str += "])*";
					str += to_string((long long)*(weightBuffer+ i));
					str += "; \n";
				}
				str += "}";
				return str.c_str();
			}

			void GaussProgram::generateAndCompileShader(int KernelSize, float Sigma)
			{
				bool generateSucessful = false;
				if(GLEW_VERSION_2_0)
				{
					//Set up a handle to our shader program
					m_shaderID = glCreateProgram();

					//Begin Frag Shader Generation
					float * weightBuffer = new float[KernelSize];
					GetGaussCoeffs(weightBuffer, Sigma, KernelSize);
					fragID = glCreateShader(GL_FRAGMENT_SHADER);
					const char * shaderSource = generateShaderSource(weightBuffer, KernelSize);
					glShaderSource(fragID, 1, &shaderSource, 0);
					glCompileShader(fragID);
					int status;
					glGetShaderiv(fragID, GL_COMPILE_STATUS, &status);
					if(status != GL_TRUE)
					{
						Logger::logError("Frag Shader did not compile");
					}
					//End Frag Shader Generation
					//Begin Vertex Shader Generation
					
					
					delete weightBuffer;
				}
				else
				{
					Logger::logError("GL version not sufficient");
				}
			}

			void GaussProgram::init()
			{
				generateAndCompileShader(kernelSize, sigma);
			}

			void GaussProgram::attachShader(Shader *shader)
			{
				Logger::logError("Attaching custom shaders not supported by Gauss Program");
			}
	}
}
