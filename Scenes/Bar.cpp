
#include "Bar.h"

using namespace std;

Bar::Bar() {
    vector<QVector2D> points;
    vector<std::pair<int, int>> indices;

    for (float i = 0; i < 10; i++) {
        points.emplace_back(-1.0f + i * 0.2f, -0.1f);
    }

    for (float i = 0; i < 10; i++) {
        points.emplace_back(-1.0f + i * 0.2f,  0.1f);
    }

    for (int i=0; i<9; i++) {
        indices.push_back(make_pair(i,i+1));
        indices.push_back(make_pair(10+i,10+i+1));

        indices.push_back(make_pair(0+i,10+i));
    }
    indices.push_back(make_pair(9,19));

    Body &body = AddBody(points, indices, 1.0f);
    body.particles[0].invMass = 0.0f;
}
