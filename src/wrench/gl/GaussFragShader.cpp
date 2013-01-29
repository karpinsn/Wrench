
#include "GaussFragShader.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define PI 3.14159265


wrench::gl::GaussFragShader::GaussFragShader()
{
	kernelSize = -1;
	sigma = -1;
}

wrench::gl::GaussFragShader::GaussFragShader(int KernelSize)
{
    kernelSize = KernelSize;
	sigma = (float)KernelSize/3.0f;
	initalized = false;
}

wrench::gl::GaussFragShader::GaussFragShader(int KernelSize, float Sigma)
{
	kernelSize = KernelSize;
	sigma  = Sigma;
	initalized = false;
}

wrench::gl::GaussFragShader::~GaussFragShader()
{
	if (initalized)
    glDeleteShader(m_shaderID);
}


void GetGaussCoeffs(float * buffer, float sigma, int kernelSize)
{
    float sum = 0;
    for (int i = 0; i < kernelSize; i++)
    {
        // Got this formula from wikipedia, worked in test code, needs checked for transcription errors
        *(buffer + i) = (1 / sqrt(2.0 * PI * pow(sigma,2.0f))) * exp(-1 * (pow(i - (kernelSize/2),2.0)/(2.0*pow(sigma, 2.0f))));
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
	str += "#version 130\n precision highp float; \n uniform sampler2D image; uniform float kernel[";
	str += static_cast<ostringstream*>( &(ostringstream() << KernelSize) )->str();
	str += "];\n in vec2 fragTexCoordOffset[";
	str += static_cast<ostringstream*>( &(ostringstream() << KernelSize) )->str();
	str += "];\n out vec4 filteredImage; \n void main(void) \n { \n";
    for(int i = 0; i < KernelSize; i ++)
    {
        char line[256];
        sprintf(line,"filteredImage += texture2D(image, fragTexCoordOffset[ %d])*%f.4; \n",i,*(weightBuffer+(i % KernelSize)));
        str += line;
    }
	str += "}";
	return str.c_str();
}

bool wrench::gl::GaussFragShader::generateAndCompileShader(int KernelSize, float Sigma)
{
    bool generateSucessful = false;
    if(GLEW_VERSION_2_0)
    {
        float * weightBuffer = new float[KernelSize];
        GetGaussCoeffs(weightBuffer, Sigma, KernelSize);
        m_shaderID = glCreateShader(GL_FRAGMENT_SHADER);
        const char * shaderSource = generateShaderSource(weightBuffer, KernelSize);
		glShaderSource(m_shaderID, 1, &shaderSource, 0);

		glCompileShader(m_shaderID);

        delete weightBuffer;
    }
    return generateSucessful;
}

bool wrench::gl::GaussFragShader::init()
{
	if(kernelSize != -1 && sigma != -1)
	{
		return generateAndCompileShader(kernelSize, sigma);
	}
	else
	{
		Logger::logError("Parameters Kernel Size and/or Sigma not initalized!");
		return false;
	}
}

bool wrench::gl::GaussFragShader::init(int KernelSize)
{
	this->kernelSize = KernelSize;
	if (this->sigma == -1)
	{
		sigma = (float)KernelSize/3.0f;
	}
	return generateAndCompileShader(kernelSize, sigma);
}

bool wrench::gl::GaussFragShader::init(int KernelSize, float Sigma)
{
	this->kernelSize = KernelSize;
	this->sigma = Sigma;
	return generateAndCompileShader(kernelSize, sigma);
}

GLuint wrench::gl::Shader::shaderID(void)
{
    return m_shaderID;
}

bool wrench::gl::Shader::_validateShader(GLuint shader, const string &filename = "")
{
    bool validShader = false;

    const unsigned int BUFFER_SIZE = 512;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    GLsizei length = 0;

    glGetShaderInfoLog(shader, BUFFER_SIZE, &length, buffer);
    validShader = length <= 0;

    if (!validShader)
    {
        Logger::logError("Error compiling shader: " + filename);
    }

    return validShader;
}
