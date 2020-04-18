#include <functional>
#include <iostream>

#include <emscripten.h>
#include <SDL.h>

#define GL_GLEXT_PROTOTYPES 1
#include <SDL_opengles2.h>

#include "Renderer.h"
#include "Scene.h"

#include "Scenes/Bar.h"

std::function<void()> loop;
void main_loop() { loop(); }

Bar scene;
Renderer renderer;
Particle *dragging = nullptr;
float draggingInvMass = 0.0f;

int width = 640;
int height = 640;

bool play = true;
bool step = false;

// the function called by the javascript code
extern "C" void EMSCRIPTEN_KEEPALIVE show_stretch(bool value) { renderer.showStretch = value; }
extern "C" void EMSCRIPTEN_KEEPALIVE show_stretch_forces(bool value) { renderer.showStretchForces = value; }
extern "C" void EMSCRIPTEN_KEEPALIVE show_bend_forces(bool value) { renderer.showBendForces = value; }

void mouseMoveEvent(const SDL_Event &event)
{
    float2 pos(event.button.x / float(width), event.button.y / float(height));
    pos = (pos * float2(2.0f, -2.0f) - float2(1.0f, -1.0f)) * renderer.invStretch;

    if (dragging) {
        dragging->position = pos;
    }
}

void mousePressEvent(const SDL_Event &event)
{
    float2 pos(event.button.x / float(width), event.button.y / float(height));
    pos = (pos * float2(2.0f, -2.0f) - float2(1.0f, -1.0f)) * renderer.invStretch;

    dragging = nullptr;
    float minDistance = std::numeric_limits<float>::max();
    for (Particle &particle : scene.bodies[0].particles) {
        float dist = length(particle.position - pos);
        if (dist < 0.03 && dist<minDistance) {
            minDistance = dist;
            dragging = &particle;
            draggingInvMass = dragging->invMass;
            dragging->invMass = 0.0f;
        }
    }
}

void mouseReleaseEvent(const SDL_Event &)
{
    if (dragging) {
        dragging->invMass = draggingInvMass;
    }
    dragging = nullptr;
}


int main()
{
    SDL_Window *window;
    SDL_Renderer *sdl_renderer;
    SDL_CreateWindowAndRenderer(width, height, 0, &window, &sdl_renderer);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
     
    
    renderer.scene = &scene;
    try
    {      
        renderer.initialize();
	renderer.resizeGL(width, height);
    }
    catch (std::runtime_error &e) {
        std::cout << e.what()  << std::endl;
        return -1;
    }    

    loop = [&]
    {
	SDL_Event event;
	if( SDL_PollEvent( &event ) ) {
	    switch (event.type) {
                case SDL_MOUSEBUTTONDOWN: mousePressEvent(event); break;
                case SDL_MOUSEBUTTONUP: mouseReleaseEvent(event); break;
                case SDL_MOUSEMOTION: mouseMoveEvent(event); break;
            }
        }

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (play || step) {
            scene.Step();
            step = false;
        } else {
            scene.UpdateForces();
        }
        renderer.Render();

        SDL_GL_SwapWindow(window);
    };

    emscripten_set_main_loop(main_loop, 0, true);

    return EXIT_SUCCESS;
}

