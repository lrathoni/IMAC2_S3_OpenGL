#version 330 core

in vec2 vTexCoords; // Coordonnées de texture du sommet
in vec2 vFragPosition;

out vec3 fFragColor;

uniform vec3 uColor;
uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main() {
  vec4 texture = texture(uEarthTexture,vTexCoords) + texture(uCloudTexture,vTexCoords);
  fFragColor = vec3(texture);
};
