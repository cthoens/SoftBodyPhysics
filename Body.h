#pragma once

#include "Particle.h"

#include <QVector2D>

#include <vector>

class Body {
public:
    float stiffness = 2.0f;
    float bendStiffness = 6.0f;
    float friction = 0.95f;
    float bendFriction = 0.95f;

    std::vector<Particle> particles;

    Body(std::vector<QVector2D> const& points, std::vector<std::pair<int, int>> const& indices, float mass);

    void Step(float elapsed);

private:
    void Bake(std::vector<QVector2D> const& points, std::vector<std::pair<int, int>> const& indices, float mass);
};
