#version 300 es

layout(location = 0) in vec3 inPosition; // ok

uniform mat4 modelMatrix; // OK
uniform mat4 viewMatrix;  // OK
uniform mat4 projMatrix;  // OK
uniform float time;       // OK

// out vec3 viewDir;   // a direção que estou olhando
// out vec4 fragColor; // a cor de sauda
// out vec3 normal;
void main() {
  // Implementação do método de Gerstner para simulação de ondas
  float A = 0.1; // Amplitude das ondas
  float k = 0.5; // Número de onda
  float w = 1.0; // Frequência angular

  float perturbationY = 0.05 * cos(2.0 * 3.14 * 0.2 * inPosition.y);

  vec3 waveOffset = vec3(0, A * sin(k * (inPosition.y + perturbationY) + w * time), 0.0);

  // Calcula a posição final do vértice
  //vec4 finalPosition = vec4(inPosition, 0.0) + vec4(waveOffset, 0.0);

  // Calcula o vetor normal a esse ponto
  // float dz_dy = A * k * cos(k * (inPosition.y + perturbationY) + w * time);

  // Normaliza a normal
  //vec3 normal = normalize(vec3(-dz_dy, 1.0, 0.0));

  // Calcula a direção de visualização do vértice
  // viewDir = -vec3(modelViewMatrix * finalPosition);

  // // Define a posição final do vértice
  gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPosition, 1.0);
}