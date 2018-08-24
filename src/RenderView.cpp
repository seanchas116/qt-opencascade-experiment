#include "RenderView.h"
#include "Shader.h"
#include "Mesh.h"
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <QKeyEvent>

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();

    _shader = Shader::fromFiles(":/Default.vert", ":/Default.frag");
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

    auto P = glm::perspective(glm::radians(90.f), float(width()) / float(height()), 0.1f, 100.f);
    auto V = glm::lookAt(_cameraPos, _cameraPos + glm::vec3(0, 0, -1), glm::vec3(0, 1, 1));
    _shader->setUniform("MVP", P * V);

    _shader->bind();
    _mesh->draw();
}

void RenderView::keyPressEvent(QKeyEvent *event) {
    _pressedKeys.insert(event->key());
    switch (event->key()) {
    case Qt::Key_W:
        _cameraPos += glm::vec3(0, 0, -1);
        break;
    case Qt::Key_S:
        _cameraPos += glm::vec3(0, 0, 1);
        break;
    case Qt::Key_A:
        _cameraPos += glm::vec3(-1, 0, 0);
        break;
    case Qt::Key_D:
        _cameraPos += glm::vec3(1, 0, 0);
        break;
    }
    update();
}

void RenderView::keyReleaseEvent(QKeyEvent *event) {
    _pressedKeys.remove(event->key());
}
