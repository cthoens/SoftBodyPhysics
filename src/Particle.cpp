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
        float2 dir = position - link.particle.position;
        float distance = length(dir);
        float elongation = distance - link.relaxedDistance;
        float2 force = normalize(dir) * -elongation * body.stiffness;
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
        float2 v1 = links[linkIndex].particle.position - position;
        float2 v2 = links[nextLinkIndex].particle.position - position;
        float link1AngleToX = float(atan2(v1.y, v1.x));
        float link2AngleToX = float(atan2(v2.y, v2.x));
        float angleToNext = link2AngleToX - link1AngleToX;
        if (angleToNext < 0) {
            angleToNext += 2 * float(M_PI);
        }
        float elongation = angleToNext - links[linkIndex].relaxedAngle;

        {
            float2 dir = normalize(position - links[linkIndex].particle.position);
            float2 perpDir(-dir.y, dir.x);
            float2 force = perpDir * -elongation * body.bendStiffness;
            links[linkIndex].particle.bendForce += force;
            bendForce -= force;
        }

        {
            float2 dir = normalize(position - links[nextLinkIndex].particle.position);
            float2 perpDir(-dir.y, dir.x);
            float2 force = perpDir * elongation * body.bendStiffness;
            links[nextLinkIndex].particle.bendForce += force;
            bendForce -= force;
        }
    }
}

void Particle::StepStretch(float elapsed)
{
    float2 acceleration = totalForce * invMass;
    float2 distance = 0.5f * acceleration * elapsed * elapsed + velocity * elapsed;
    velocity += acceleration * elapsed;
    velocity *= body.friction;
    position += distance;
}

void Particle::StepBend(float elapsed)
{
    float2 acceleration = bendForce * invMass;
    float2 distance = 0.5f * acceleration * elapsed * elapsed + bendVelocity * elapsed;
    bendVelocity += acceleration * elapsed;
    bendVelocity *= body.bendFriction;
    position += distance;
}
