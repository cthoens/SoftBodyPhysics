#pragma once

#include <vector>
#include <SDL_opengles2.h>

#include "../linalg.h"
using namespace linalg::aliases;

class SingleColorProgram {
public:
    GLuint m_vbo_vertices = 0;
    GLuint m_shaderProgram = 0;
    GLuint m_posAttr = 0;
    GLuint m_colUniform = 0;
    GLuint m_matrixUniform = 0;

    void initialize();
    void bind();
    void release();

    void SetColor(const float3 &color);
    void SetProjection(const float4x4 &projection);
    void SetVertices(std::vector<float2> &vertices);
};
