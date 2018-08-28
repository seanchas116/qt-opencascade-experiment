#pragma once
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QSet>
#include <glm/glm.hpp>
#include <cmath>

namespace ShapeEditor {

class Shape;

class RenderView : public QOpenGLWidget, protected QOpenGLExtraFunctions {
public:
    RenderView(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

private:
    std::shared_ptr<Shape> _shape;
    glm::mat4 _projectionMatrix;
    glm::mat4 _cameraMatrix;
    glm::vec3 _cameraPos = {0, 0, 10};
    glm::vec3 _cameraRotation = {0, -M_PI, 0};
    glm::vec3 _cameraDirection;
    glm::vec3 _cameraRight;
    glm::vec3 _cameraUp;

    void updateCameraMatrix();

    enum class DragMode {
        None,
        Move,
        Rotate,
    };
    DragMode _dragMode = DragMode::None;
    QPoint _lastMousePos;
    QSet<int> _pressedKeys;
};

}
