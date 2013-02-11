#ifndef _GAUSS_PRGM_
#define _GAUSS_PRGM_

#include"ShaderProgram.h"

using namespace std;

namespace wrench
{
	namespace gl
	{
			class GaussProgram : ShaderProgram
			{
			public:
					GaussProgram(int KenrelSize);
					GaussProgram(int KernelSize, float Sigma);
					~GaussProgram();
					virtual void init() override;
					virtual void attachShader(Shader *shader) override;

					virtual void uniform(const string name, const int data) override;
                    virtual void uniform(const string name, const float data) override;
					virtual void uniform(const string name, const float *data, int count) override;
                    virtual void uniform(const string name, const glm::vec3 data) override;
                    virtual void uniform(const string name, const glm::vec4 data) override;
                    virtual void uniform(const string name, const glm::mat3 data) override;
                    virtual void uniform(const string name, const glm::mat4 data) override;
                    virtual void uniformMat4(const string name, GLboolean transpose, const GLfloat *value) override;
                    virtual void bindAttributeLocation(const string name, const GLuint index) override;
			
			private:
				int kernelSize;
				float sigma;
				GLuint fragID;
				GLuint vertID;

				void generateAndCompileShader(int KernelSize, float Sigma);
			};
	}
}

#endif