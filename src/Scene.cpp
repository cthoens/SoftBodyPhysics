#include "Scene.h"

using namespace std;

Body& Scene::AddBody(vector<float2> const& particles, vector<std::pair<int, int>> const& indices, float mass) {
    bodies.emplace_back(particles, indices, mass);
    return bodies.back();
}

void Scene::Step() {
    for (auto &body : bodies) {
        body.Step(1.0f / 120.0f);
    }
}

void Scene::UpdateForces() {
    for (auto &body : bodies) {
        body.UpdateForces();
    }
}
