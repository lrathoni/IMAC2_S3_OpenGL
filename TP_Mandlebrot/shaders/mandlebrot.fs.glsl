#version 330 core

vec2 complexSqr(vec2 z) {
  float a = z.x;
  float b = z.y;

  vec2 result;

  result.x = a*a - b*b;
  result.y = 2*a*b;

  return result;
}

in vec2 vFragPosition;

out vec3 fFragColor;

void main() {
  float j =0;
  float N = 50;
  vec2 c = vec2(vFragPosition.x, vFragPosition.y);
  vec2 z = c;
  if (length(z) > 2) {
    fFragColor = vec3(1,1,1);
    return;
  }
  for( int i=0; i<N; i++) {
    j++;
    z = complexSqr(z) + c;
    if ( length(z) > 2) {
      fFragColor = vec3(1-j/N,1-0.2-j/N,1-0.5-j/N);
      return;
    }
  }
  fFragColor = vec3(0.1, 0.2, 0.4);
  return;
};