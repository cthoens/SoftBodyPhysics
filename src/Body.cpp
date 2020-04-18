#include "Body.h"

#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

Body::Body(vector<float2> const& points, std::vector<std::pair<int, int>> const& indices, float mass)
{
    Bake(points, indices, mass);
}

void Body::Step(float elapsed) {
    for (auto &particle : particles) {
        particle.totalForce = float2();
        particle.bendForce = float2();
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

void Body::UpdateForces()
{
    for (auto &particle : particles) {
        particle.totalForce = float2();
        particle.bendForce = float2();
    }

    for (auto &particle : particles) {
        particle.UpdateStretchForce();
    }

    for (auto &particle : particles) {
        particle.UpdateBendForce();
    }
}

class TempLink {
public:
    class Particle *particle;
    float relaxedDistance;
    float angleToX;

    static bool compareLinkAngles(TempLink const& link1, TempLink const& link2)
    {
        return (link1.angleToX < link2.angleToX);
    }
};

class TempParticle {
public:
    float2 position;
    vector<TempLink> links;

    void AddLink(Particle &particle, float relaxedDistance)
    {
        float2 v1 = particle.position - position;
        float angleToX = atan2(v1.y, v1.x);
        TempLink link = {&particle, relaxedDistance, angleToX};
        links.push_back(link);
    }

    void CreateRealLinks(Particle &p) {
        std::sort(links.begin(), links.end(), TempLink::compareLinkAngles);

        int next = 1;
        for (auto const& tmpLink : links) {
            float nextAngelToX = links[(next++) % int(links.size())].angleToX;
            float angleToNext =  nextAngelToX - tmpLink.angleToX;
            if (angleToNext < 0) {
                angleToNext += 2 * float(M_PI);
            }
            Particle &particle = *(tmpLink.particle);
            Link link = {particle, tmpLink.relaxedDistance, angleToNext};
            p.links.push_back(link);
        }
    }
};


void Body::Bake(vector<float2> const& points, std::vector<std::pair<int, int>> const& indices, float mass) {
    assert(points.size() >= 2);

    float invMass = 1.0f / mass;

    vector<TempParticle> tempParticles;
    tempParticles.reserve(points.size());
    particles.reserve(points.size());

    for (auto &point : points) {
        Particle particle(*this);
        particle.position = point;
        particle.velocity = float2(0.0f, 0.0f);
        particle.bendVelocity = float2(0.0f, 0.0f);
        particle.invMass = invMass;
        particles.push_back(particle);
        tempParticles.push_back({particle.position});
    }

    for (auto &index : indices) {
        Particle &firstParticle = particles[index.first];
        Particle &secondParticle = particles[index.second];
        TempParticle &firstTempParticle = tempParticles[index.first];
        TempParticle &secondTempParticle = tempParticles[index.second];

        float relaxedDistance = length(firstParticle.position - secondParticle.position);

        firstTempParticle.AddLink(secondParticle, relaxedDistance);
        secondTempParticle.AddLink(firstParticle, relaxedDistance);
    }

    auto particleIt = particles.begin();
    auto tempParticleIt = tempParticles.begin();
    for (; particleIt < particles.end(); particleIt++, tempParticleIt++) {
        (*tempParticleIt).CreateRealLinks(*particleIt);
    }

    /*
    auto previousIt = particles.begin();
    auto currentIt = ++particles.begin();
    auto nextIt = ++++particles.begin();

    while (currentIt != particles.end())
    {
        Particle &previous = *previousIt;
        Particle &current = *currentIt;

        float restingDistance = length(current.position - previous.position);

        Link previousLink = {current, restingDistance};
        previousIt->links.push_back(previousLink);

        Link currentLink = { previous, restingDistance };
        currentIt->links.push_back(currentLink);

        if (nextIt != particles.end())
        {
            Particle &next = *nextIt;
            float2 v1 = previous.position - current.position;
            float2 v2 = next.position - current.position;
            float restingAngle = (float)(atan2(v2.y(), v2.x()) - atan2(v1.y(), v1.x()));
            if (restingAngle < 0) restingAngle += 2 * float(M_PI);
            current.restingAngles.push_back(restingAngle);
            nextIt++;
        }

        previousIt++;
        currentIt++;
    }*/
}
