#include "TextureDisplay.h"

wrench::gl::utils::TextureDisplay::TextureDisplay(void)
{
  m_hasBeenInit = false;
}

void wrench::gl::utils::TextureDisplay::init()
{
  if(!m_hasBeenInit)
  {
    _cacheQuad();
    _initShaders();
    m_hasBeenInit = true;
  }
}

void wrench::gl::utils::TextureDisplay::_initShaders(void)
{
  m_textureDisplay.init();
  m_textureDisplay.attachShader(new Shader(GL_VERTEX_SHADER, "Shaders/TextureDisplay.vert"));
  m_textureDisplay.attachShader(new Shader(GL_FRAGMENT_SHADER, "Shaders/TextureDisplay.frag"));
  m_textureDisplay.bindAttributeLocation("vert", 0);
  m_textureDisplay.bindAttributeLocation("vertTexCoord", 1);

  m_textureDisplay.link();
  m_textureDisplay.uniform("texture2Display", 0);
  OGLStatus::logOGLErrors("CameraGLContext - _initShaders()");
}

void wrench::gl::utils::TextureDisplay::draw(Texture* texture)
{
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  m_textureDisplay.bind();
  {
    texture->bind(GL_TEXTURE0);
    m_screen.draw();
  }
  m_textureDisplay.unbind();

  OGLStatus::logOGLErrors("CameraGLContext - draw(Texture)");
}

void wrench::gl::utils::TextureDisplay::_cacheQuad(void)
{
  float vertex[12]; // vertex array
  float tex[8];     // texture coord array

  vertex[0] = -1.0f; vertex[1]   =-1.0f; vertex[2]  = 0.0f;
  vertex[3] =  1.0f; vertex[4]   =-1.0f; vertex[5]  = 0.0f;
  vertex[6] =  1.0f; vertex[7]   = 1.0f; vertex[8]  = 0.0f;
  vertex[9] = -1.0f; vertex[10]  = 1.0f; vertex[11] = 0.0f;

  tex[0] = 0.0f; tex[1] = 0.0f;
  tex[2] = 1.0f; tex[3] = 0.0f;
  tex[4] = 1.0f; tex[5] = 1.0f;
  tex[6] = 0.0f; tex[7] = 1.0f;

  m_screen.init(GL_QUADS, 4);
  m_vertex.init(3, GL_FLOAT, GL_ARRAY_BUFFER);
  m_vertex.bufferData(4, vertex, GL_STATIC_DRAW);
  m_screen.addVBO(m_vertex, "vert");

  m_texCoords.init(2, GL_FLOAT, GL_ARRAY_BUFFER);
  m_texCoords.bufferData(4, tex, GL_STATIC_DRAW);
  m_screen.addVBO(m_texCoords, "vertTexCoord");
}
