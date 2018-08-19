#include "RenderView.h"
#include "Shader.h"
#include "Mesh.h"
#include <array>

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();

    _shader = Shader::fromFiles(":/Default.vert", ":/Default.frag");
    _shader->setUniform("MVP", glm::mat4(1));
    _mesh = std::make_shared<Mesh>();

    std::vector<Vertex> vertices = {
        {{0, -1, 0}, {0, 0}, {0, 0, 1}},
        {{-1, 1, 0}, {0, 0}, {0, 0, 1}},
        {{1, 1, 0}, {0, 0}, {0, 0, 1}},
    };
    std::vector<std::array<uint16_t, 3>> triangles = {
        {{0, 1, 2}}
    };
    _mesh->setTriangles(triangles);
    _mesh->setVertices(vertices);
}

void RenderView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void RenderView::paintGL() {
    glClearColor(0.6, 0.6, 0.6, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    _shader->bind();
    _mesh->draw();
}
