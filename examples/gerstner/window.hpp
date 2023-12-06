#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "camera.hpp"
#include "ground.hpp"

struct Vertex {
  glm::vec3 position;

  friend bool operator==(Vertex const &, Vertex const &) = default;
};

class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void onUpdate() override;

private:
  glm::ivec2 m_viewportSize{};
  abcg::Timer m_timer;
  GLuint m_VAO{};
  GLuint m_VBO{};
  GLuint m_EBO{};
  GLuint m_program{};

  std::array<glm::vec3, 4> const m_vertices{
      {{-1, 0, -1}, {1, 0, -1}, {-1, 0, 1}, {1, 0, 1}}};

  std::array<unsigned int, 6> const m_indices{0, 2, 1, 1, 2, 3};

  GLint m_viewMatrixLocation{};
  GLint m_projMatrixLocation{};
  GLint m_modelMatrixLocation{};
  GLint m_colorLocation{};
  GLint m_time{};
  GLint m_viewDir{};

  Camera m_camera;
  float m_dollySpeed{};
  float m_truckSpeed{};
  float m_panSpeed{};
  float m_delay{20};
  float time{0.01};
  Ground m_ground;

  // std::vector<Vertex> m_vertices;
  // std::vector<GLuint> m_indices;

  void loadModelFromFile(std::string_view path);
};

#endif