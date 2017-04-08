attribute highp vec4 posAttr;
attribute highp vec3 colorAttr;
uniform highp mat4 matrix;
out vec3 drawColor;
void main() {
   gl_Position = matrix * posAttr;
   drawColor = colorAttr;
}
