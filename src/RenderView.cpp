#include "RenderView.h"
#include "Shader.h"
#include "Mesh.h"

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();

    _shader = Shader::fromFiles(":/Default.vert", ":/Default.frag");
}

void RenderView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    glClearColor(0.6, 0.6, 0.6, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderView::paintGL() {
}
