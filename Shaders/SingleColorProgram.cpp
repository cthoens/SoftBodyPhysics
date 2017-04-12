#include "SingleColorProgram.h"

#include "Renderer.h"

#include <exception>

using namespace std;

void SingleColorProgram::initialize() {
    addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/singleColor.vertex.shader");
    addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/singleColor.fragment.shader");
    link();
    m_posAttr = attributeLocation("posAttr");
    m_colUniform = uniformLocation("drawColor");
    m_matrixUniform = uniformLocation("matrix");

    if (m_posAttr == -1 || m_colUniform==-1 || m_matrixUniform==-1)
        throw runtime_error("Error loading Shader");
}

void SingleColorProgram::bind() {
    QOpenGLShaderProgram::bind();
    gl.glEnableVertexAttribArray(m_posAttr);
}

void SingleColorProgram::release() {
    gl.glDisableVertexAttribArray(m_posAttr);
    QOpenGLShaderProgram::release();
}

void SingleColorProgram::SetColor(QVector3D &color) {
    setUniformValue(m_colUniform, color.x(), color.y(), color.z());
}

void SingleColorProgram::SetProjection(QMatrix4x4 &projection) {
    setUniformValue(m_matrixUniform, projection);
}

void SingleColorProgram::SetVertices(std::vector<QVector2D> &vertices) {
    gl.glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices.data());
}
