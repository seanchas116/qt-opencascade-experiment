#include "RenderView.h"
#include "Shader.h"
#include "Mesh.h"
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <QKeyEvent>
#include <QtDebug>

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

    glm::vec3 direction(
        cos(_cameraRotation.x) * sin(_cameraRotation.y),
        sin(_cameraRotation.x),
        cos(_cameraRotation.x) * cos(_cameraRotation.y)
    );
    glm::vec3 right = glm::vec3(
        sin(_cameraRotation.y - float(M_PI) * 0.5f),
        0,
        cos(_cameraRotation.y - float(M_PI) * 0.5f)
    );
    glm::vec3 up = glm::cross(right, direction);

    auto V = glm::lookAt(_cameraPos, _cameraPos + direction, up);
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

void RenderView::mousePressEvent(QMouseEvent *event) {
    switch (event->button()) {
    case Qt::MiddleButton:
        _dragMode = DragMode::Move;
        break;
    case Qt::RightButton:
        _dragMode = DragMode::Rotate;
        break;
    default:
        break;
    }
    _lastMousePos = event->pos();
}

void RenderView::mouseMoveEvent(QMouseEvent *event) {
    QPoint offset = event->pos() - _lastMousePos;
    switch (_dragMode) {
    case DragMode::Move:
        break;
    case DragMode::Rotate: {
        float unit = 0.25f / 180.f * (M_PI);
        _cameraRotation -= glm::vec3(offset.y() * unit, offset.x() * unit, 0);
        update();
        break;
    }
    default:
        break;
    }
    _lastMousePos = event->pos();
}

void RenderView::mouseReleaseEvent(QMouseEvent *event) {
    _dragMode = DragMode::None;
}

void RenderView::wheelEvent(QWheelEvent *event) {
    _cameraPos += glm::vec3(0, 0, -0.01 * event->delta());
    update();
}
