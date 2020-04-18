#pragma once

#include "../Scene.h"

enum InnerSupport {
    NONE, REGULAR, CROSS
};

class Bar : public Scene {
public:
    InnerSupport innerSupport = CROSS;

    Bar();
};
