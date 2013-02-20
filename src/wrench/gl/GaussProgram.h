#ifndef _WRENCH_GL_GAUSS_PROGRAM_H_ 
#define _WRENCH_GL_GAUSS_PROGRAM_H_

#include <memory>

#include"ShaderProgram.h"

using namespace std;

namespace wrench
{
  namespace gl
  {
	class GaussProgram : public ShaderProgram
	{
	private:
	  const int m_kernelSize;
	  const float m_sigma;

	  bool m_horizontalFilter;

	  #ifdef USE_VRJ
		vrj::opengl::ContextData<GLuint> vrjFragID;
		vrj::opengl::ContextData<GLuint> vrjVertID;
		#define m_fragID (*vrjFragID)
		#define m_vertID (*vrjVertID)
	  #else
		GLuint m_fragID;
		GLuint m_vertID;
	  #endif

	public:
	  GaussProgram(int KenrelSize);
	  GaussProgram(int KernelSize, float Sigma);
	  virtual ~GaussProgram( );

	  void setImageDimensions(int width, int height);
	  virtual void init() override;
	  virtual void attachShader(Shader *shader) override;//This is to make sure we don't mess anything up
	  
	  /**
		* Flips the Gaussian filter so that it is the opposite of what it was
		* either horizontal or vertical. This is used for the second render pass,
		* since this uses a seperable gaussian filter
		*/
	  void flipFilter( void );
	private:
	  const int _attachVertShader( );
	  const int _attachFragShader( );
	  unique_ptr<float[]> _GetGaussCoeffs( );
	};
  }
}
#endif // _WRENCH_GL_GAUSS_PROGRAM_H_