#include "Scene.h"

using namespace std;

Body& Scene::AddBody(vector<QVector2D> const& particles, vector<std::pair<int, int>> const& indices, float mass) {
    bodies.emplace_back(particles, indices, mass);
    return bodies.back();
}
