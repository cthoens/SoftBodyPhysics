#pragma once

#include "Renderer.h"

#include <QGLWidget>

class RenderWidget : public QGLWidget
{
public:
    boolean play = true;
    RenderWidget(QWidget *parent);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    Renderer renderer;
};
