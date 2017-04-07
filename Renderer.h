#pragma once

#include <QVector2D>
#include <QMatrix4x4>
#include <QtGui/QOpenGLFunctions>

#include <vector>

struct Body {
    std::vector<class Particle> particles;

    void Render(class Renderer &renderer) const;
    void Step(float elapsed);
};

class Renderer : public QOpenGLFunctions {
public:
    int m_posAttr = 0;
    int m_colUniform = 0;
    int m_matrixUniform = 0;
    QVector2D invStretch;

    Renderer();

    void initialize(QObject *parent);
    void resizeGL(float w, float h);
    void Step();
    void Render();

    void SetColor(QVector3D color);

    Body& AddBody(std::vector<class Particle> &particles);

private:
    QMatrix4x4 projection;
    class QOpenGLShaderProgram *m_program;

    std::vector<Body> bodies;

    void RenderBody(Body const& body);
};
