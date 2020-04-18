#pragma once

#include <vector>
#include <SDL_opengles2.h>

#include "../linalg.h"
using namespace linalg::aliases;

class MultiColorProgram {
public:
    GLuint m_vbo_vertices = 0, m_vbo_colors = 0;
    GLuint m_shaderProgram = 0;
    GLuint m_posAttr = 0;
    GLuint m_colorAttr = 0;
    GLuint m_matrixUniform = 0;

    void initialize();
    void bind();
    void release();

    void SetProjection(float4x4 &projection);
    void SetVertices(std::vector<float2> &vertices, std::vector<float3> &colors);
};
