#version 330 core

in vec3 vPosition_vs; // Position du sommet transformé dans l'espace View
in vec3 vNormal_vs; // Normale du sommet transformé dans l'espace View

out vec3 fFragColor;

void main() {
  fFragColor = normalize(vNormal_vs);
};
