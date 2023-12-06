#ifndef GROUND_HPP_
#define GROUND_HPP_

#include "abcgOpenGL.hpp"

class Ground {
public:
  void create(GLuint program);
  void paint();
  void destroy();

  float time{1.0f};
  float amplitude{0.46};
  float wavelength{1.0};
  float speed{1.0};
  int m_delay{50};
  bool animate_time{false};

private:
  glm::vec3 u_lightDirection = glm::vec3(1.0, 0.5, 5.0);
  GLuint m_VAO{};
  GLuint m_VBO{};
  abcg::Timer m_timer;
  GLint m_modelMatrixLoc{};
  GLint m_colorLoc{};
};

#endif