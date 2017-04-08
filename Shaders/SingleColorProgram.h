#pragma once

#include <QtGui/QOpenGLShaderProgram>

#include <vector>

class SingleColorProgram : public QOpenGLShaderProgram {
public:
    int m_posAttr = 0;
    int m_colUniform = 0;
    int m_matrixUniform = 0;

    void initialize();
    void bind();
    void release();

    void SetColor(QVector3D &color);
    void SetProjection(QMatrix4x4 &projection);
    void SetVertices(std::vector<QVector2D> &vertices);
};
