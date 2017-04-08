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
    multiColorProgram.initialize();
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

    for (auto const& body : scene->bodies) {
        RenderBody(body);
    }
}

bool showStretch = true;
QVector3D relaxedColor(1.0f, 1.0f, 1.0f);
QVector3D stretchedColor(1.0f, 0.0f, 0.0f);
QVector3D compressedColor(0.0f, 0.0f, 1.0f);

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

    if (!showStretch) {
        singleColorProgram.bind();
        singleColorProgram.SetProjection(projection);
        singleColorProgram.SetVertices(points);
        singleColorProgram.SetColor(QVector3D(1.0f, 1.0f, 1.0f));
        glDrawArrays(GL_LINES, 0, 2 * linkCount);
        singleColorProgram.release();
    } else {
        multiColorProgram.bind();
        multiColorProgram.SetProjection(projection);

        vector<QVector3D> colors;
        for (auto &particle : particles) {
            for (auto &link: particle.links) {
                float length = (link.particle.position - particle.position).length();
                float stretch = (max(min(length / link.relaxedDistance, 1.2f), 0.8f) -1.0f) * 5.0f;

                QVector3D linkColor;
                if (stretch < 0.0f) {
                    stretch *= -1.0f;
                    linkColor = compressedColor * stretch + relaxedColor * (1.0f - stretch);
                } else {
                    linkColor = stretchedColor * stretch + relaxedColor * (1.0f - stretch);
                }
                colors.emplace_back(linkColor);
                colors.emplace_back(linkColor);
            }
        }

        multiColorProgram.SetVertices(points, colors);
        glDrawArrays(GL_LINES, 0, 2 * linkCount);
        multiColorProgram.release();
    }

    // StretchForces
    singleColorProgram.bind();
    singleColorProgram.SetProjection(projection);

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

    singleColorProgram.release();
}
