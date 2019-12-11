#version 330 core

in vec2 vTexCoords; // Coordonnées de texture du sommet
in vec3 vPosition_vs;
in vec3 vNormal_vs;

out vec3 fFragColor;

//Light management
uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

//Working in the view space = product(vec3, ViewMatrix)
uniform vec3 uLightDir_vs; //wi =  normalize (uLightDir_vs)
uniform vec3 uLightIntensity; //Li

vec3 blinnPhong(vec3 uKd,vec3 uKs, float uShininess, vec3 uLightDir_vs, vec3 uLightIntensity) {
    vec3 wi = normalize(uLightDir_vs);
    vec3 w0= normalize(-vPosition_vs);
    vec3 halfVector = (wi + w0)/2;
    return uLightIntensity * ( uKd * dot(wi,vNormal_vs) + uKs * pow(dot(halfVector,vNormal_vs),uShininess));
}

void main() {
  fFragColor = blinnPhong(uKd, uKs, uShininess, uLightDir_vs, uLightIntensity);
};
