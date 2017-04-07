#pragma once

#include "Body.h"

#include <vector>

class Scene {
public:
    std::vector<Body> bodies;

    Body& AddBody(std::vector<class QVector2D> const& particles, std::vector<std::pair<int, int>> const& indices, float mass);
};
