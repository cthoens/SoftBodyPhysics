#include "Renderer.h"

#include "Scene.h"
#include "Body.h"
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

void Renderer::Step() {
    for (auto &body : scene->bodies) {
        body.Step(1.0f / 120.0f);
    }
}

void Renderer::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_program->bind();
    m_program->setUniformValue(m_matrixUniform, projection);

    for (auto const& body : scene->bodies) {
        RenderBody(body);
    }

    m_program->release();
}

void Renderer::SetColor(QVector3D color) {
    m_program->setUniformValue(m_colUniform, color.x(), color.y(), color.z());
}

void Renderer::RenderBody(Body const& body) {
    std::vector<Particle> const& particles = body.particles;

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

    glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, &points[0]);
    glEnableVertexAttribArray(0);

    int particleCount = int(particles.size());

    SetColor(QVector3D(1.0f, 1.0f, 1.0f));
    glDrawArrays(GL_LINE_STRIP, 0, particleCount);

    SetColor(QVector3D(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_LINES, particleCount, 2 * particleCount);

    SetColor(QVector3D(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_LINES, 3 * particleCount, 2 * particleCount);

    glDisableVertexAttribArray(0);
}
