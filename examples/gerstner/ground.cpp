#include "ground.hpp"
#include "abcgOpenGL.hpp"

void Ground::create(GLuint program) {
  // Unit quad on the xz plane
  std::array<glm::vec3, 4> vertices{{{-0.5f, 0.0f, +0.5f},
                                     {-0.5f, 0.0f, -0.5f},
                                     {+0.5f, 0.0f, +0.5f},
                                     {+0.5f, 0.0f, -0.5f}}};

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Create VAO and bind vertex attributes
  abcg::glGenVertexArrays(1, &m_VAO);
  abcg::glBindVertexArray(m_VAO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  auto const positionAttribute{
      abcg::glGetAttribLocation(program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
  abcg::glBindVertexArray(0);

  // Save location of uniform variables
  m_modelMatrixLoc = abcg::glGetUniformLocation(program, "modelMatrix");
  m_colorLoc = abcg::glGetUniformLocation(program, "color");
}

void Ground::paint() {
  abcg::glBindVertexArray(m_VAO);
  if (m_timer.elapsed() < m_delay / 1000.0)
    return;
  m_timer.restart();
  if (animate_time) {
    if (time > 2.0) {
      time = 0.0;
    } else {
      time += 0.01;
    }
    if (wavelength > 2.0) {
      wavelength = 1.0;
    } else {
      wavelength += 0.01;
    }
  }
  auto const N{150};
  for (auto const z : iter::range(-N, N + 1)) {
    for (auto const x : iter::range(-N, N + 1)) {
      // Set model matrix as a translation matrix

      glm::vec3 v_position = glm::vec3(static_cast<float>(x) / 12.5, 0.0f,
                                       static_cast<float>(z) / 12.5);
      float waveHeight =
          amplitude *
              glm::sin(
                  glm::dot(glm::vec2(u_lightDirection.x, u_lightDirection.y),
                           glm::vec2(v_position.x, v_position.z))) *
              (6.283185 / wavelength) +
          time * speed;
      glm::vec3 perturbedPosition =
          v_position + glm::vec3(0.0, waveHeight, 0.0);

      glm::vec3 finalColor =
          glm::vec3(35.0 / 255.0, 35.0 / 255.0, 142.0 / 255.0) *
          (waveHeight / amplitude);

      glm::mat4 model{1.0f};
      model = glm::scale(model, glm::vec3(0.1f));
      model = glm::translate(model,
                             glm::vec3(perturbedPosition.x, perturbedPosition.y,
                                       perturbedPosition.z));
      abcg::glUniformMatrix4fv(m_modelMatrixLoc, 1, GL_FALSE, &model[0][0]);

      abcg::glUniform4f(m_colorLoc, finalColor.x, finalColor.y, finalColor.z,
                        1.0f);

      abcg::glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
  }

  abcg::glBindVertexArray(0);
}

void Ground::destroy() {
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}