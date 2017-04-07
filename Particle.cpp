#include "Particle.h"

#include <cassert>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

float Particle::stiffness = 2.0f;
float bendStiffness = 0.2f;
float friction = 0.95f;
float bendFriction = 0.95f;

Particle::Particle()
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
        QVector2D force = dir.normalized() * -elongation * stiffness;
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
        float elongation = angle - restingAngles[0];

        {
            QVector2D dir = (position - links[0].particle.position).normalized();
            QVector2D perpDir(-dir.y(), dir.x());
            QVector2D force = perpDir * -elongation * bendStiffness;
            links[0].particle.bendForce += force;
            bendForce -= force;
        }

        {
            QVector2D dir = (position - links[1].particle.position).normalized();
            QVector2D perpDir(-dir.y(), dir.x());
            QVector2D force = perpDir * elongation * bendStiffness;
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
    velocity *= friction;
    position += distance;
}

void Particle::StepBend(float elapsed)
{
    QVector2D acceleration = bendForce * invMass;
    QVector2D distance = 0.5f * acceleration * elapsed * elapsed + bendVelocity * elapsed;
    bendVelocity += acceleration * elapsed;
    bendVelocity *= bendFriction;
    position += distance;
}

vector<Particle> Particle::Bake(vector<QVector2D> points, float mass) {
    assert(points.size() >= 2);

    float invMass = 1.0f / mass;

    vector<Particle> particles;
    particles.reserve(points.size());

    for (auto &point : points) {
        Particle particle;
        particle.position = point;
        particle.velocity = QVector2D(0.0f, 0.0f);
        particle.bendVelocity = QVector2D(0.0f, 0.0f);
        particle.invMass = invMass;
        particles.push_back(particle);
    }

    auto previousIt = particles.begin();
    auto currentIt = ++particles.begin();
    auto nextIt = ++++particles.begin();

    while (currentIt != particles.end())
    {
        Particle &previous = *previousIt;
        Particle &current = *currentIt;

        float restingDistance = (current.position - previous.position).length();

        Link previousLink = {current, restingDistance};
        previousIt->links.push_back(previousLink);

        Link currentLink = { previous, restingDistance };
        currentIt->links.push_back(currentLink);

        if (nextIt != particles.end())
        {
            Particle &next = *nextIt;
            QVector2D v1 = previous.position - current.position;
            QVector2D v2 = next.position - current.position;
            float restingAngle = (float)(atan2(v2.y(), v2.x()) - atan2(v1.y(), v1.x()));
            if (restingAngle < 0) restingAngle += 2 * float(M_PI);
            current.restingAngles.push_back(restingAngle);
            nextIt++;
        }

        previousIt++;
        currentIt++;
    }

    return particles;
}
