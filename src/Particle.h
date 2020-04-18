#pragma once

#include <vector>

#include "linalg.h"
using namespace linalg::aliases;

class Link {
public:
    class Particle &particle;
	float relaxedDistance;
    float relaxedAngle;
};

class Particle
{
public:
    class Body& body;
    float2 position;
    float2 velocity;
    float2 bendVelocity;
    float2 totalForce;
    float2 bendForce;
    std::vector<Link> links;
    float invMass;

    Particle(Body& body);
    ~Particle();

    void UpdateStretchForce();
    void UpdateBendForce();
    void StepStretch(float timestep);
    void StepBend(float timestep);
    static std::vector<Particle> Bake(std::vector<float2> points, float mass);
};

