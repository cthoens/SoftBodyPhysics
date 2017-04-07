#include "Renderer.h"

#include "Particle.h"

#include <QtGui/QOpenGLShaderProgram>

#include <exception>

using namespace std;

static const char* vertexShaderSource =
        "attribute highp vec4 posAttr;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {"
        "   gl_Position = matrix * posAttr;\n"
        "}";

static const char* fragmentShaderSource =
        "uniform vec3 drawColor;"
        "out vec4 frag_colour;"
        "void main() {"
        "  frag_colour = vec4(drawColor, 1.0);"
        "}";

Renderer::Renderer() {
}

void Renderer::initialize(QObject *parent) {
    m_program = new QOpenGLShaderProgram(parent);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colUniform = m_program->uniformLocation("drawColor");
    m_matrixUniform = m_program->uniformLocation("matrix");

    if (m_posAttr == -1 || m_colUniform==-1 || m_matrixUniform==-1)
        throw runtime_error("Error loading Shader");
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
    m_program->setUniformValue(m_matrixUniform, projection);
}

Body& Renderer::AddBody(vector<Particle> &particles) {
    bodies.push_back({std::move(particles)});
    return bodies.back();
}

void Renderer::Step() {
    for (auto &body : bodies) {
        body.Step(1.0f / 60.0f);
    }
}

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue(m_matrixUniform, projection);

    for (auto &body : bodies) {
        body.Render(*this);
    }

    m_program->release();
}

void Renderer::SetColor(QVector3D color) {
    m_program->setUniformValue(m_colUniform, color.x(), color.y(), color.z());
}

void Body::Render(Renderer &renderer) const {
    vector<QVector2D> points;
    points.reserve(particles.size());
    for (auto &particle : particles) {
            points.push_back(particle.position);
    }

    for (auto &particle : particles) {
            points.push_back(particle.position);
            points.push_back(particle.position + .5f * particle.bendForce);
    }

    for (auto &particle : particles) {
            points.push_back(particle.position);
            points.push_back(particle.position + .5f * particle.totalForce);
    }

    renderer.glVertexAttribPointer(renderer.m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, &points[0]);
    renderer.glEnableVertexAttribArray(0);

    int particleCount = int(particles.size());

    renderer.SetColor(QVector3D(1.0f, 1.0f, 1.0f));
    renderer.glDrawArrays(GL_LINE_STRIP, 0, particleCount);

    renderer.SetColor(QVector3D(1.0f, 0.0f, 0.0f));
    renderer.glDrawArrays(GL_LINES, particleCount, 2 * particleCount);

    renderer.SetColor(QVector3D(0.0f, 1.0f, 0.0f));
    renderer.glDrawArrays(GL_LINES, 3 * particleCount, 2 * particleCount);

    renderer.glDisableVertexAttribArray(0);
}

void Body::Step(float elapsed) {
    for (auto &particle : particles) {
        particle.totalForce = QVector2D();
        particle.bendForce = QVector2D();
    }

    for (auto &particle : particles) {
        particle.UpdateStretchForce();
    }

    for (auto &particle : particles) {
        particle.StepStretch(elapsed);
    }

    for (auto &particle : particles) {
        particle.UpdateBendForce();
    }

    for (auto &particle : particles) {
        particle.StepBend(elapsed);
    }
}
