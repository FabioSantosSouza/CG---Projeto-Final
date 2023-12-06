#version 300 es

precision mediump float;

out vec4 fragColor;
// in vec3 viewDir; // Direção de visualização do vértice
// in vec3 normal;

void main() {
  // Configuração de iluminação simples

  // vec3 lightDir = normalize(viewDir);               // Direção da fonte de
  // luz float diffuse = max(dot(normal, lightDir), 0.0);  // Componente difusa
  // vec3 reflectedLight = reflect(-lightDir, normal); // Vetor de luz refletida
  // float specular = pow(max(dot(reflectedLight, normalize(viewDir)), 0.0),
  //                      32.0); // Componente especular
  // vec3 baseColor = vec3(0.0, 0.1, 1.0);
  // // Cor final do fragmento (iluminação difusa + especular)
  // // vec3 color = vec3(0.0, 0.5, 1.0); // Cor da água
  // // fragColor = vec4(color * (diffuse + specular), 1.0);
  // // Altura do vértice afeta a intensidade da cor
  // float heightFactor =
  //     (gl_FragCoord.z / gl_FragCoord.w) * 0.1; // Ajuste conforme necessário

  // // Cor final do fragmento (iluminação difusa + especular)
  // vec3 color = baseColor * (diffuse + specular);

  // // Ajuste a cor com base na altura do vértice
  fragColor = vec4(1, 1, 0, 1);
}
