#pragma once

#include "Shaders/SingleColorProgram.h"
#include "Shaders/MultiColorProgram.h"

#include <QVector2D>
#include <QMatrix4x4>
#include <QtGui/QOpenGLFunctions>

#include <vector>

class Renderer : public QOpenGLFunctions {
public:
    bool showStretchForces = false;
    bool showBendForces = false;
    QVector2D invStretch;
    class Scene *scene = nullptr;

    Renderer();

    void initialize();
    void resizeGL(float w, float h);
    void Render();

    class Body& AddBody(std::vector<class Particle> &particles);

private:
    QMatrix4x4 projection;
    SingleColorProgram singleColorProgram;
    MultiColorProgram multiColorProgram;

    void RenderBody(Body const& body);
    void RenderForces(Body const& body);
};

extern Renderer gl;
