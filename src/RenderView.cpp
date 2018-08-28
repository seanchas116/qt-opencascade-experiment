#include "RenderView.hpp"
#include "MakeBottle.hpp"
#include "Shape.hpp"
#include <QKeyEvent>
#include <QtDebug>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace ShapeEditor {

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();

    updateCameraMatrix();
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    _shape = std::make_shared<Shape>();
    _shape->setOCCShape(MakeBottle(10, 10, 4));
}

void RenderView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    _projectionMatrix = glm::perspective(glm::radians(90.f), float(width()) / float(height()), 0.1f, 100.f);
}

void RenderView::paintGL() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shape->render(_cameraMatrix, _projectionMatrix);
}

void RenderView::keyPressEvent(QKeyEvent *event) {
    _pressedKeys.insert(event->key());
}

void RenderView::keyReleaseEvent(QKeyEvent *event) {
    _pressedKeys.remove(event->key());
}

void RenderView::mousePressEvent(QMouseEvent *event) {
    bool rotateKey = _pressedKeys.contains(Qt::Key_Space) && event->modifiers() & Qt::ShiftModifier;
    bool moveKey = _pressedKeys.contains(Qt::Key_Space) && !(event->modifiers() & Qt::ShiftModifier);

    if (moveKey || event->button() == Qt::MiddleButton) {
        _dragMode = DragMode::Move;
    } else if (rotateKey || event->button() == Qt::RightButton) {
        _dragMode = DragMode::Rotate;
    } else {
        return;
    }
    _lastMousePos = event->pos();
}

void RenderView::mouseMoveEvent(QMouseEvent *event) {
    QPoint offset = event->pos() - _lastMousePos;
    switch (_dragMode) {
    case DragMode::Move: {
        _cameraPos += _cameraUp * float(offset.y()) * 0.05f;
        _cameraPos += -_cameraRight * float(offset.x()) * 0.05f;
        updateCameraMatrix();
        break;
    }
    case DragMode::Rotate: {
        float unit = 0.25f / 180.f * float(M_PI);
        _cameraRotation -= glm::vec3(offset.y() * unit, offset.x() * unit, 0);
        updateCameraMatrix();
        break;
    }
    default:
        break;
    }
    _lastMousePos = event->pos();
}

void RenderView::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event)
    _dragMode = DragMode::None;
}

void RenderView::wheelEvent(QWheelEvent *event) {
    _cameraPos += _cameraDirection * (0.01f * event->delta());
    updateCameraMatrix();
}

void RenderView::updateCameraMatrix() {
    _cameraDirection = glm::vec3(
        cos(_cameraRotation.x) * sin(_cameraRotation.y),
        sin(_cameraRotation.x),
        cos(_cameraRotation.x) * cos(_cameraRotation.y)
    );
    _cameraRight = glm::vec3(
        sin(_cameraRotation.y - float(M_PI) * 0.5f),
        0,
        cos(_cameraRotation.y - float(M_PI) * 0.5f)
    );
    _cameraUp = glm::cross(_cameraRight, _cameraDirection);
    _cameraMatrix = glm::lookAt(_cameraPos, _cameraPos + _cameraDirection, _cameraUp);
    update();
}

}
