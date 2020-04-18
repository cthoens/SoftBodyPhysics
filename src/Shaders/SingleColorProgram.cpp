#include "SingleColorProgram.h"

#include "../Renderer.h"

#include <exception>
#include <iostream>

using namespace std;

// Shader sources
const GLchar* vertexSource1 =
    "attribute highp vec4 posAttr;      \n"
    "uniform highp mat4 matrix;         \n"
    "void main() {                      \n"
    "   gl_Position = matrix * posAttr; \n"
    "}                                  \n";

const GLchar* fragmentSource1 =
    "precision mediump float;               \n"
    "uniform vec3 drawColor;                \n"
    "void main() {                          \n"
    "  gl_FragColor = vec4(drawColor, 1.0); \n"
    "}                                      \n";


void SingleColorProgram::initialize() {
    glGenBuffers(1, &m_vbo_vertices);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource1, nullptr);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource1, nullptr);
    glCompileShader(fragmentShader);

    // Check if shaders compiled successfully
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
	throw runtime_error("Single: Failed to compile vertex shader");
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
	throw runtime_error("Single: Failed to compile fragment shader");
    }

    // Link the vertex and fragment shader into a shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);

    m_posAttr = glGetAttribLocation(m_shaderProgram, "posAttr");
    m_colUniform = glGetUniformLocation(m_shaderProgram, "drawColor");
    m_matrixUniform = glGetUniformLocation(m_shaderProgram, "matrix");

    if (m_posAttr == -1 || m_colUniform==-1 || m_matrixUniform==-1)
        throw runtime_error("Single: Error loading Shader");
}

void SingleColorProgram::bind() {
    glEnableVertexAttribArray(m_posAttr);
}

void SingleColorProgram::release() {
    glDisableVertexAttribArray(m_posAttr);
}

void SingleColorProgram::SetColor(const float3 &color) {
    glUseProgram(m_shaderProgram);
    glUniform3f(m_colUniform, color.x, color.y, color.z);
}

void SingleColorProgram::SetProjection(const float4x4 &projection) {
    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(m_matrixUniform, 1, false, (GLfloat*) &projection);
}

void SingleColorProgram::SetVertices(std::vector<float2> &vertices) {
    glUseProgram(m_shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float2), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);
}
