#include "RenderView.h"
#include "Shader.h"
#include "Mesh.h"
#include <array>
#include <glm/gtc/matrix_transform.hpp>

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
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
    auto P = glm::perspective(glm::radians(90.f), float(width()) / float(height()), 0.1f, 100.f);

    glm::vec3 cameraPos(0, -10, 10);
    glm::vec3 cameraTarget(0);

    auto V = glm::lookAt(cameraPos, cameraTarget, glm::vec3(0, 1, 1));

    _shader->setUniform("MVP", P * V);

    glViewport(0, 0, w, h);
}

void RenderView::paintGL() {
    glClearColor(0.6, 0.6, 0.6, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    _shader->bind();
    _mesh->draw();
}
