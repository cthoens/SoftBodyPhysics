#pragma once

#include "Body.h"

#include <vector>

#include "linalg.h"
using namespace linalg::aliases;

class Scene {
public:
    std::vector<Body> bodies;

    Body& AddBody(std::vector<float2> const& particles, std::vector<std::pair<int, int>> const& indices, float mass);

    void Step();
    void UpdateForces();
};
