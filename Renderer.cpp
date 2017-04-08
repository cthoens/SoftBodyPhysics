#include "Renderer.h"

#include "Scene.h"
#include "Body.h"
#include "Particle.h"

using namespace std;

Renderer gl;

Renderer::Renderer() {
}

void Renderer::initialize() {
    singleColorProgram.initialize();
}

void Renderer::resizeGL(float w, float h) {
    glViewport(0, 0, w, h);
    projection.setToIdentity();
    if (w>h) {
        invStretch = QVector2D(w/h, 1.0f);
        projection.scale(h/w, 1.0f);
    } else {
        invStretch = QVector2D(1.0f, h/w);
        projection.scale(1.0f, w/h);
    }
    singleColorProgram.SetProjection(projection);
}

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    singleColorProgram.bind();
    singleColorProgram.SetProjection(projection);

    for (auto const& body : scene->bodies) {
        RenderBody(body);
    }

    singleColorProgram.release();
}

void Renderer::RenderBody(Body const& body) {
    std::vector<Particle> const& particles = body.particles;

    vector<QVector2D> points;
    points.reserve(particles.size());

    int linkCount = 0;
    for (auto &particle : particles) {
        for (auto &link: particle.links) {
            points.push_back(particle.position);
            points.push_back(link.particle.position);
            linkCount++;
        }
    }

    singleColorProgram.SetVertices(points);
    singleColorProgram.SetColor(QVector3D(1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINES, 0, 2 * linkCount);

    // StretchForces
    points.clear();
    for (auto &particle : particles) {
        points.push_back(particle.position);
        points.push_back(particle.position + .5f * particle.bendForce);
    }
    singleColorProgram.SetColor(QVector3D(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 2 * int(particles.size()));

    // BendForces
    points.clear();
    for (auto &particle : particles) {
        points.push_back(particle.position);
        points.push_back(particle.position + .5f * particle.totalForce);
    }
    singleColorProgram.SetColor(QVector3D(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 0, 2 * int(particles.size()));

    glDisableVertexAttribArray(0);
}
