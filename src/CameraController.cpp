#include "CameraController.hpp"
#include <QEvent>

CameraController::CameraController(QObject *parent) : QObject(parent) {
}

bool CameraController::eventFilter(QObject *object, QEvent *event) {
    Q_UNUSED(object)
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        break;
    }
    case QEvent::MouseButtonRelease: {
        break;
    }
    case QEvent::MouseMove: {
        break;
    }
    case QEvent::Wheel: {
        break;
    }
    default:
        break;
    }
    return false;
}
