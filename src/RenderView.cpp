#include "RenderView.h"

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();
}

void RenderView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glClearColor(0.6, 0.6, 0.6, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderView::paintGL() {
}
