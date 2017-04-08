#pragma once

#include <QVector2D>
#include <QMatrix4x4>
#include <QtGui/QOpenGLFunctions>

#include <vector>

class Renderer : public QOpenGLFunctions {
public:    
    QVector2D invStretch;
    class Scene *scene = nullptr;

    Renderer();

    void initialize(QObject *parent);
    void resizeGL(float w, float h);
    void Render();

    void SetColor(QVector3D color);

    class Body& AddBody(std::vector<class Particle> &particles);

private:
    int m_posAttr = 0;
    int m_colUniform = 0;
    int m_matrixUniform = 0;
    QMatrix4x4 projection;
    class QOpenGLShaderProgram *m_program;

    void RenderBody(Body const& body);
};
