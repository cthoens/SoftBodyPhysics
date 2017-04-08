attribute highp vec4 posAttr;
uniform highp mat4 matrix;
void main() {
   gl_Position = matrix * posAttr;
}
