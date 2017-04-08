#pragma once

#include <QtGui/QOpenGLShaderProgram>

#include <vector>

class MultiColorProgram : public QOpenGLShaderProgram {
public:
    int m_posAttr = 0;
    int m_colorAttr = 0;
    int m_matrixUniform = 0;

    void initialize();
    void bind();
    void release();

    void SetProjection(QMatrix4x4 &projection);
    void SetVertices(std::vector<QVector2D> &vertices, std::vector<QVector3D> &colors);
};
