#include "Particle.h"
#include "Body.h"

#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Particle::Particle(Body& body) :
    body(body)
{
}

Particle::~Particle()
{
}

void Particle::UpdateStretchForce()
{
    for (auto &link : links) {
        QVector2D dir = position - link.particle.position;
        float distance = dir.length();
        float elongation = distance - link.relaxedDistance;
        QVector2D force = dir.normalized() * -elongation * body.stiffness;
        totalForce += force;
    }
}

void Particle::UpdateBendForce()
{
    if (links.size() == 2) {
        QVector2D v1 = links[0].particle.position - position;
        QVector2D v2 = links[1].particle.position - position;
        float angle = (float)(atan2(v2.y(), v2.x()) - atan2(v1.y(), v1.x()));
        if (angle < 0) angle += 2 * float(M_PI);
        float elongation = angle - links[0].relaxedAngle;

        {
            QVector2D dir = (position - links[0].particle.position).normalized();
            QVector2D perpDir(-dir.y(), dir.x());
            QVector2D force = perpDir * -elongation * body.bendStiffness;
            links[0].particle.bendForce += force;
            bendForce -= force;
        }

        {
            QVector2D dir = (position - links[1].particle.position).normalized();
            QVector2D perpDir(-dir.y(), dir.x());
            QVector2D force = perpDir * elongation * body.bendStiffness;
            links[1].particle.bendForce += force;
            bendForce -= force;
        }
    }
}

void Particle::StepStretch(float elapsed)
{
    QVector2D acceleration = totalForce * invMass;
    QVector2D distance = 0.5f * acceleration * elapsed * elapsed + velocity * elapsed;
    velocity += acceleration * elapsed;
    velocity *= body.friction;
    position += distance;
}

void Particle::StepBend(float elapsed)
{
    QVector2D acceleration = bendForce * invMass;
    QVector2D distance = 0.5f * acceleration * elapsed * elapsed + bendVelocity * elapsed;
    bendVelocity += acceleration * elapsed;
    bendVelocity *= body.bendFriction;
    position += distance;
}
