#include "Body.h"

#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Body::Body(vector<QVector2D> const& points, std::vector<std::pair<int, int>> const& indices, float mass)
{
    Bake(points, indices, mass);
}

void Body::Step(float elapsed) {
    for (auto &particle : particles) {
        particle.totalForce = QVector2D();
        particle.bendForce = QVector2D();
    }

    for (auto &particle : particles) {
        particle.UpdateStretchForce();
    }

    for (auto &particle : particles) {
        particle.StepStretch(elapsed);
    }

    for (auto &particle : particles) {
        particle.UpdateBendForce();
    }

    for (auto &particle : particles) {
        particle.StepBend(elapsed);
    }
}

void Body::Bake(vector<QVector2D> const& points, std::vector<std::pair<int, int>> const& indices, float mass) {
    assert(points.size() >= 2);

    float invMass = 1.0f / mass;

    particles.reserve(points.size());
    for (auto &point : points) {
        Particle particle(*this);
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
}
