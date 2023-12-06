#include "window.hpp"
#include "camera.hpp"

#include <unordered_map>

// Explicit specialization of std::hash for Vertex
template <> struct std::hash<Vertex> {
  size_t operator()(Vertex const &vertex) const noexcept {
    auto const h1{std::hash<glm::vec3>()(vertex.position)};
    return h1;
  }
};

void Window::onEvent(SDL_Event const &event) {
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_dollySpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_dollySpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_panSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_panSpeed = 1.0f;
    if (event.key.keysym.sym == SDLK_q)
      m_truckSpeed = -1.0f;
    if (event.key.keysym.sym == SDLK_e)
      m_truckSpeed = 1.0f;
  }
  if (event.type == SDL_KEYUP) {
    if ((event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w) &&
        m_dollySpeed > 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s) &&
        m_dollySpeed < 0)
      m_dollySpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a) &&
        m_panSpeed < 0)
      m_panSpeed = 0.0f;
    if ((event.key.keysym.sym == SDLK_RIGHT ||
         event.key.keysym.sym == SDLK_d) &&
        m_panSpeed > 0)
      m_panSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_q && m_truckSpeed < 0)
      m_truckSpeed = 0.0f;
    if (event.key.keysym.sym == SDLK_e && m_truckSpeed > 0)
      m_truckSpeed = 0.0f;
  }
}

void Window::onCreate() {
  auto const &assetsPath{abcg::Application::getAssetsPath()};

  abcg::glClearColor(0, 0, 0, 1);

  // Enable depth buffering
  abcg::glEnable(GL_DEPTH_TEST);
  // abcg::glEnable(GL_CULL_FACE);
  abcg::glCullFace(GL_BACK);

  // Create program
  m_program =
      abcg::createOpenGLProgram({{.source = assetsPath + "lookat.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "lookat.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // m_ground.create(m_program);

  // Get location of uniform variables
  m_viewMatrixLocation = abcg::glGetUniformLocation(m_program, "viewMatrix");
  m_projMatrixLocation = abcg::glGetUniformLocation(m_program, "projMatrix");
  m_modelMatrixLocation = abcg::glGetUniformLocation(m_program, "modelMatrix");
  m_colorLocation = abcg::glGetUniformLocation(m_program, "color");
  m_time = abcg::glGetUniformLocation(m_program, "time");
  // m_viewDir = abcg::glGetUniformLocation(m_program, "viewDir");

  glm::vec3 topLeft{-1.0f, 0.0f, -1.0f};
  glm::vec3 topRight{1.0f, 0.0f, -1.0f};
  glm::vec3 bottomLeft{-1.0f, 0.0f, 1.0f};
  glm::vec3 bottomRight{1.0f, 0.0f, 1.0f};

  // Número de linhas e colunas na malha quadrada
  int N{2};

  // Vetor para armazenar os vértices
  std::vector<glm::vec3> vertices;
  std::vector<unsigned int> indices;

  // Gera os vértices da malha quadrangular
  for (int i = 0; i <= N; ++i) {
    for (int j = 0; j <= N; ++j) {
      float u = static_cast<float>(j) / static_cast<float>(N);
      float v = static_cast<float>(i) / static_cast<float>(N);

      // glm::vec3 point = (1.0f - u) * (1.0f - v) * topLeft +
      //                   u * (1.0f - v) * topRight +
      //                   (1.0f - u) * v * bottomLeft + u * v * bottomRight;
      glm::vec3 point = (1.0f - v) * (1.0f - u) * topLeft +
                        v * (1.0f - u) * topRight +
                        (1.0f - v) * u * bottomLeft + v * u * bottomRight;

      vertices.push_back(point);
    }
  }
  // Gerando os índices
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      int vertexIndex = i * (N + 1) + j;

      // Triângulo 1
      indices.push_back(vertexIndex);
      indices.push_back(vertexIndex + 1);
      indices.push_back(vertexIndex + N + 1);

      // Triângulo 2
      indices.push_back(vertexIndex + 1);
      indices.push_back(vertexIndex + N + 1);
      indices.push_back(vertexIndex + N + 2);
    }
  }

  // Exibe os vértices gerados
  for (const auto &vertex : vertices) {
    fmt::print("({:+.2f}, {:+.2f}, {:+.2f})\n", vertex.x, vertex.y, vertex.z);
  }

  // Exibe os indices dos vértices gerados
  for (const auto &index : indices) {
    fmt::print("({},)\n", index);
  }

  // Generate VBO
  abcg::glGenBuffers(1, &m_VBO);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_EBO);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};
  // Create VAO
  abcg::glGenVertexArrays(1, &m_VAO);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_VAO);

  // auto const positionAttribute{
  //     abcg::glGetAttribLocation(m_program, "inPosition")};
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  abcg::glVertexAttribPointer(positionAttribute, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Window::onPaint() {
  // Clear color buffer and depth buffer
  abcg::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);
  // fmt::print("({:+.2f}, {:+.2f}, {:+.2f})\n", m_camera.m_at.x,
  // m_camera.m_at.y, m_camera.m_at.z);

  // fmt::print("({:+.2f}\n", time);

  // if (m_timer.elapsed() < m_delay / 1000.0)
  //   return;
  // m_timer.restart();
  // if (time > 2.0) {
  //   time = 0.0;
  // } else {
  //   time += 0.01;
  // }
  abcg::glUseProgram(m_program);

  // Set uniform variables for viewMatrix and projMatrix
  // These matrices are used for every scene object
  glm::mat4 modelMatrix{1};

  abcg::glUniformMatrix4fv(m_viewMatrixLocation, 1, GL_FALSE,
                           &m_camera.getViewMatrix()[0][0]);
  abcg::glUniformMatrix4fv(m_projMatrixLocation, 1, GL_FALSE,
                           &m_camera.getProjMatrix()[0][0]);

  abcg::glUniformMatrix4fv(m_modelMatrixLocation, 1, GL_FALSE,
                           &modelMatrix[0][0]);
  abcg::glUniform1f(m_time, time);

  // glUniform3f(m_viewDir, m_camera.m_at.x, m_camera.m_at.y, m_camera.m_at.z);

  abcg::glBindVertexArray(m_VAO);

  abcg::glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  // Draw ground
  // m_ground.paint();

  abcg::glUseProgram(0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  // Our own ImGui widgets go below
  {
    // Window begin
    ImGui::Begin("Options");
    ImGui::PushItemWidth(300);
    ImGui::SliderFloat("Amplitude:", &m_ground.amplitude, 0.0f, 2.0f);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(300);
    ImGui::SliderFloat("Tamanho da Onda:", &m_ground.wavelength, 1.0f, 2.0f);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(300);
    ImGui::SliderFloat("Velocidade: ", &m_ground.speed, 0.0f, 10.0f);
    ImGui::PopItemWidth();
    ImGui::PushItemWidth(300);
    ImGui::SliderFloat("Time: ", &time, 0.0f, 2.0f);
    ImGui::PopItemWidth();
    ImGui::Checkbox("Animar Tempo? (0.01)", &m_ground.animate_time);
    ImGui::PushItemWidth(140);
    ImGui::SliderInt("Delay", &m_ground.m_delay, 0, 200, "%d ms");
    ImGui::PopItemWidth();
    // More static text
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0 / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // Window end
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;
  m_camera.computeProjectionMatrix(size);
}

void Window::onDestroy() {
  m_ground.destroy();

  abcg::glDeleteProgram(m_program);
  abcg::glDeleteBuffers(1, &m_EBO);
  abcg::glDeleteBuffers(1, &m_VBO);
  abcg::glDeleteVertexArrays(1, &m_VAO);
}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Update LookAt camera
  m_camera.dolly(m_dollySpeed * deltaTime);
  m_camera.truck(m_truckSpeed * deltaTime);
  m_camera.pan(m_panSpeed * deltaTime);
}