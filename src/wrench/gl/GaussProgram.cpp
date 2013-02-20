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

			const char * generateVertShaderSource(int KernelSize)
			{
				string str;
				str+="#version 130\n precision highp float; out vec2 fragTexCoordOffset[";
				str+= to_string((long long)KernelSize);
				str+= "];\n void main(void)\n {\n";
				for(int i = 0; i < KernelSize; i++)
				{
					str+= "fragTexCoordOffset[";
					str+= to_string((long long) i);
					str+= "] = ";
					str+= to_string((long long)(i -(KernelSize/2)));
					str+=  ";\n";
				}
				str += "}";
				return str.c_str();
			}

			const char * generateShaderSource(float * weightBuffer, int KernelSize)
			{
				string str;
				str += "#version 130\n precision highp float;\n uniform sampler2D image; uniform bool h_or_V;\n in vec2 fragTexCoordOffset[";
				str += to_string((long long)KernelSize);
				str += "];\n out vec4 filteredImage; \n void main(void) \n { \n";
				str += "if(h_or_v == true)//true = horizontal\n {\n";
				for(int i = 0; i < KernelSize; i ++)
				{
					//line looks like this : 
					//filteredImage += texture2D(image, fragTexCoordOffset[i]) * (*(weightBuffer + i));\n
					//i and *(weightBuffer + i) get hard-coded as values in the shader code
					str += "filteredImage += texture2D(image, vec2(gl_FragCoord.x + glfragTexCoordOffset[";
					str += to_string((long long)i);
					str += "], gl_FragCoord.y))*";
					str += to_string((long long)*(weightBuffer+ i));
					str += "; \n";
				}
				for(int i = 0; i < KernelSize; i ++)
				{
					//line looks like this : 
					//filteredImage += texture2D(image, fragTexCoordOffset[i]) * (*(weightBuffer + i));\n
					//i and *(weightBuffer + i) get hard-coded as values in the shader code
					str += "filteredImage += texture2D(image, vec2(gl_FragCoord.x, gl_FragCoord.y + glfragTexCoordOffset[";
					str += to_string((long long)i);
					str += "]))*";
					str += to_string((long long)*(weightBuffer+ i));
					str += "; \n";
				}
				str += "} \n else\n{\n";
				str+=" }\n}";
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
					vertID = glCreateShader(GL_VERTEX_SHADER);
					shaderSource = generateVertShaderSource(KernelSize);
					glShaderSource(vertID, 1, &shaderSource, 0);
					glGetShaderiv(vertID, GL_COMPILE_STATUS, &status);
					if(status != GL_TRUE)
					{
						Logger::logError("Vert Shader did not compile");
					}
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
