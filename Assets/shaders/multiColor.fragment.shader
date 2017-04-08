in vec3 drawColor;
out vec4 frag_colour;
void main() {
  frag_colour = vec4(drawColor, 1.0);
}