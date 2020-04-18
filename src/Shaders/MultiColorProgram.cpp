#include "MultiColorProgram.h"

#include "../Renderer.h"

#include <cassert>
#include <exception>

#include <SDL_opengles2.h>

using namespace std;

// Shader sources
const GLchar* vertexSource2 =
    "attribute highp vec4 posAttr;      \n"
    "attribute highp vec3 colorAttr;    \n"
    "uniform highp mat4 matrix;         \n"
    "varying vec3 drawColor;            \n"
    "void main() {                      \n"
    "   gl_Position = matrix * posAttr; \n"
    "   drawColor = colorAttr;          \n"
    "}\n";

const GLchar* fragmentSource2 =
    "precision mediump float;               \n"
    "varying vec3 drawColor;                \n"
    "void main() {                          \n"
    "  gl_FragColor = vec4(drawColor, 1.0); \n"
    "}\n";

void MultiColorProgram::initialize() {
    glGenBuffers(1, &m_vbo_vertices);
    glGenBuffers(1, &m_vbo_colors);

    // Create and compile the vertex shader    
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource2, nullptr);
    glCompileShader(vertexShader);
    

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource2, nullptr);
    glCompileShader(fragmentShader);
    
    // Check if shaders compiled successfully
    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
	throw runtime_error("Multi: Failed to compile vertex shader");
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
	throw runtime_error("Multi: Failed to compile fragment shader");
    }


    // Link the vertex and fragment shader into a shader program
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, vertexShader);
    glAttachShader(m_shaderProgram, fragmentShader);
    glLinkProgram(m_shaderProgram);
    glUseProgram(m_shaderProgram);

    m_posAttr = glGetAttribLocation(m_shaderProgram, "posAttr");
    m_colorAttr = glGetAttribLocation(m_shaderProgram, "colorAttr");
    m_matrixUniform = glGetUniformLocation(m_shaderProgram, "matrix");

    if (m_posAttr == -1 || m_colorAttr==-1 || m_matrixUniform==-1)
        throw runtime_error("Multi: Error loading Shader");
}

void MultiColorProgram::bind() {
    glEnableVertexAttribArray(m_posAttr);
    glEnableVertexAttribArray(m_colorAttr);
}

void MultiColorProgram::release() {
    glDisableVertexAttribArray(m_colorAttr);
    glDisableVertexAttribArray(m_posAttr);
}

void MultiColorProgram::SetProjection(float4x4 &projection) {
    glUseProgram(m_shaderProgram);
    glUniformMatrix4fv(m_matrixUniform, 1, false, (GLfloat*) &projection);
}

void MultiColorProgram::SetVertices(std::vector<float2> &vertices, std::vector<float3> &colors) {
    glUseProgram(m_shaderProgram);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float2), vertices.data(), GL_STATIC_DRAW);    
    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float3), colors.data(), GL_STATIC_DRAW);    
    glVertexAttribPointer(m_colorAttr, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
