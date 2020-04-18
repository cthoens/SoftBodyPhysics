#pragma once

#include "Particle.h"

#include <vector>

#include "linalg.h"
using namespace linalg::aliases;

class Body {
public:
    float stiffness = 30.0f;
    float bendStiffness = 9.0f;
    float friction = 0.95f;
    float bendFriction = 0.95f;

    std::vector<Particle> particles;

    Body(std::vector<float2> const& points, std::vector<std::pair<int, int>> const& indices, float mass);

    void Step(float elapsed);
    void UpdateForces();

private:
    void Bake(std::vector<float2> const& points, std::vector<std::pair<int, int>> const& indices, float mass);
};
