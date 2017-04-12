#include "RenderWidget.h"

#include "Body.h"
#include "Particle.h"

#include <QTimer>
#include <QMouseEvent>
#include <QVector2D>
#include <QtCore/qmath.h>

#include <vector>

using namespace std;

QTimer timer;

RenderWidget::RenderWidget(QWidget *parent) :
    QGLWidget(parent),
    renderer(gl)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(10);
    renderer.scene = &scene;
}

void RenderWidget::initializeGL()
{
    gl.initializeOpenGLFunctions();
    renderer.initialize();
}

void RenderWidget::resizeGL(int w, int h)
{
    const qreal retinaScale = devicePixelRatio();
    renderer.resizeGL(float(w * retinaScale), float(h * retinaScale));
}

void RenderWidget::paintGL()
{
    if (play || step) {
        scene.Step();
        step = false;
    } else {
        scene.UpdateForces();
    }
    renderer.Render();
}

Particle *dragging = nullptr;
float draggingInvMass = 0.0f;

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    QVector2D pos(event->x() / float(width()), event->y() / float(height()));
    pos = (pos * devicePixelRatio() * QVector2D(2.0f, -2.0f) - QVector2D(1.0f, -1.0f)) * renderer.invStretch;

    if (dragging) {
        dragging->position = pos;
    }
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    QVector2D pos(event->x() / float(width()), event->y() / float(height()));
    pos = (pos * devicePixelRatio() * QVector2D(2.0f, -2.0f) - QVector2D(1.0f, -1.0f)) * renderer.invStretch;

    dragging = nullptr;
    for (Particle &particle : scene.bodies[0].particles) {
        if ((particle.position - pos).length() < 0.03) {
            dragging = &particle;
            draggingInvMass = dragging->invMass;
            dragging->invMass = 0.0f;
            break;
        }
    }
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (dragging) {
        dragging->invMass = draggingInvMass;
    }
    dragging = nullptr;
}

