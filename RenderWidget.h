#pragma once

#include "Renderer.h"
#include "Scene.h"

#include "Scenes/Bar.h"

#include <QGLWidget>

class RenderWidget : public QGLWidget
{
public:
    Bar scene;
    Renderer &renderer;

    boolean play = true;
    boolean step = false;

    RenderWidget(QWidget *parent);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
};
