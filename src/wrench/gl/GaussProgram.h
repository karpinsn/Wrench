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
					GaussProgram(int KernelSize, int Sigma);
					~GaussProgram();
					void init();
					void atachShader(Shader *shader);

			};
	}
}

#endif