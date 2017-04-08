#pragma once

#include <QtGui/QVector2D>

#include <vector>

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
    QVector2D position;
    QVector2D velocity;
    QVector2D bendVelocity;
    QVector2D totalForce;
    QVector2D bendForce;
    std::vector<Link> links;
    float invMass;

    Particle(Body& body);
    ~Particle();

    void UpdateStretchForce();
    void UpdateBendForce();
    void StepStretch(float timestep);
    void StepBend(float timestep);
    static std::vector<Particle> Bake(std::vector<QVector2D> points, float mass);
};

