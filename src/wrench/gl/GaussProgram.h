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
					virtual void attachShader(Shader *shader) override;//This is to make sure we don't mess anything up
					virtual void uniform(const string name, const bool data); //Use this for h_or_v

			
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