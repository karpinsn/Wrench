
#include "GaussFragShader.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

#define PI 3.14159265

wrench::gl::GaussFragShader::GaussFragShader(int KernelSize)
{
    GaussFragShader(KernelSize, (float)KernelSize/3.0f);
}

wrench::gl::GaussFragShader::GaussFragShader(int KernelSize, float Sigma)
{
    m_shaderID = 0;
    generateAndCompileShader(KernelSize,Sigma);
}

wrench::gl::GaussFragShader::~GaussFragShader()
{
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
//This shader is going to expect fragTexCoordOffsets as an in array, with offsets in both directions
const char * generateShaderSource(float * weightBuffer, int KernelSize)
{
    string str;
	str += "#version 130\n precision highp float; \n uniform sampler2D image; uniform float kernel[";
	str += static_cast<ostringstream*>( &(ostringstream() << KernelSize * 2) )->str();
	str += "];\n in vec2 fragTexCoordOffset[";
	str += static_cast<ostringstream*>( &(ostringstream() << KernelSize * 2) )->str();
	str += "];\n out vec4 filteredImage; \n void main(void) \n { \n";
    for(int i = 0; i < KernelSize*2; i ++)
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

        delete weightBuffer;
    }
    return generateSucessful;
}

/// This function is unimplemented and should not be used
bool wrench::gl::GaussFragShader::init(GLenum shaderType, const string &filename)
{
    return true;
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
