#include "MultiColorProgram.h"

#include "Renderer.h"

#include <cassert>
#include <exception>

using namespace std;

void MultiColorProgram::initialize() {
    addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/multiColor.vertex.shader");
    addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/multiColor.fragment.shader");
    link();
    m_posAttr = attributeLocation("posAttr");
    m_colorAttr = attributeLocation("colorAttr");
    m_matrixUniform = uniformLocation("matrix");

    if (m_posAttr == -1 || m_colorAttr==-1 || m_matrixUniform==-1)
        throw runtime_error("Error loading Shader");
}

void MultiColorProgram::bind() {
    QOpenGLShaderProgram::bind();
    gl.glEnableVertexAttribArray(m_posAttr);
    gl.glEnableVertexAttribArray(m_colorAttr);
}

void MultiColorProgram::release() {
    gl.glDisableVertexAttribArray(m_colorAttr);
    gl.glDisableVertexAttribArray(m_posAttr);
    QOpenGLShaderProgram::release();
}

void MultiColorProgram::SetProjection(QMatrix4x4 &projection) {
    setUniformValue(m_matrixUniform, projection);
}

void MultiColorProgram::SetVertices(std::vector<QVector2D> &vertices, std::vector<QVector3D> &colors) {
    gl.glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, &vertices[0]);
    gl.glVertexAttribPointer(m_colorAttr, 3, GL_FLOAT, GL_FALSE, 0, &colors[0]);
}
