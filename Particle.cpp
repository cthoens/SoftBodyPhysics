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
    int linkCount = int(links.size());
    if (linkCount < 2) {
        return;
    }

    for (int linkIndex=0; linkIndex<linkCount; linkIndex++)
    {
        int nextLinkIndex = (linkIndex + 1) % linkCount;
        QVector2D v1 = links[linkIndex].particle.position - position;
        QVector2D v2 = links[nextLinkIndex].particle.position - position;
        float link1AngleToX = float(atan2(v1.y(), v1.x()));
        float link2AngleToX = float(atan2(v2.y(), v2.x()));
        float angleToNext = link2AngleToX - link1AngleToX;
        if (angleToNext < 0) {
            angleToNext += 2 * float(M_PI);
        }
        float elongation = angleToNext - links[linkIndex].relaxedAngle;

        {
            QVector2D dir = (position - links[linkIndex].particle.position).normalized();
            QVector2D perpDir(-dir.y(), dir.x());
            QVector2D force = perpDir * -elongation * body.bendStiffness;
            links[linkIndex].particle.bendForce += force;
            bendForce -= force;
        }

        {
            QVector2D dir = (position - links[nextLinkIndex].particle.position).normalized();
            QVector2D perpDir(-dir.y(), dir.x());
            QVector2D force = perpDir * elongation * body.bendStiffness;
            links[nextLinkIndex].particle.bendForce += force;
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
