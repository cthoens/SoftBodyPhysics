#include "RenderWidget.h"

#include "Particle.h"

#include <QTimer>
#include <QMouseEvent>
#include <QVector2D>
#include <QtCore/qmath.h>

#include <vector>

using namespace std;

QTimer timer;

RenderWidget::RenderWidget(QWidget *parent) :
    QGLWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(10);
}

Body *body;

void RenderWidget::initializeGL()
{
    renderer.initializeOpenGLFunctions();
    renderer.initialize(this);

    vector<QVector2D> points;
    for (int i = 0; i < 4; i++) {
        float angle = (2.0f * float(M_PI) * i) / 10.0f;
        points.push_back(0.5f * QVector2D(cos(angle), sin(angle)));
    }
    vector<Particle> particles = Particle::Bake(points, 1.0f);

    body = &renderer.AddBody(particles);
    //body->particles[0].invMass = 0.0f;
}

void RenderWidget::resizeGL(int w, int h)
{
    const qreal retinaScale = devicePixelRatio();
    renderer.resizeGL(float(w * retinaScale), float(h * retinaScale));
}

void RenderWidget::paintGL()
{
    if (play) {
        renderer.Step();
    }
    renderer.Render();
}

Particle *dragging = nullptr;

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
    for (Particle &particle : body->particles) {
        if ((particle.position - pos).length() < 0.03) {
            dragging = &particle;
            break;
        }
    }
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *)
{
    dragging = nullptr;
}

