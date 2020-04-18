
#include "Bar.h"

#include "../linalg.h"

using namespace linalg::aliases;
using namespace std;

Bar::Bar() {
    vector<float2> points;
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
    }

    indices.push_back(make_pair(0,10));
    indices.push_back(make_pair(9,19));

    if (innerSupport == REGULAR || innerSupport == CROSS) {
        for (int i=0; i<9; i++) {
            indices.push_back(make_pair(0+i,10+i));
        }
    }

    if (innerSupport == CROSS) {
        for (int i=0; i<9; i++) {
            indices.push_back(make_pair(0+i,11+i));
            indices.push_back(make_pair(1+i,10+i));
        }
    }


    Body &body = AddBody(points, indices, 1.0f);
}
