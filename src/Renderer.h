#pragma once

#include "Shaders/SingleColorProgram.h"
#include "Shaders/MultiColorProgram.h"

#include <vector>

#include <SDL_opengles2.h>

class Renderer {
public:
    bool showStretch = false;
    bool showStretchForces = false;
    bool showBendForces = false;
    float2 invStretch;
    class Scene *scene = nullptr;

    Renderer();

    void initialize();
    void resizeGL(float w, float h);
    void Render();

    class Body& AddBody(std::vector<class Particle> &particles);

private:
    float4x4 projection;
    SingleColorProgram singleColorProgram;
    MultiColorProgram multiColorProgram;

    void RenderBody(Body const& body);
    void RenderForces(Body const& body);
};

extern Renderer gl;
