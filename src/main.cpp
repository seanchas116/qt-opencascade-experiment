#include <QApplication>
#include <QWidget>
#include <QSurfaceFormat>
#include "RenderView.hpp"

int main(int argc, char *argv[]) {
    using namespace ShapeEditor;

    QApplication a(argc, argv);

    QSurfaceFormat glFormat;

    // Request OpenGL 3.3 compatibility or OpenGL ES 3.0.
    if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
        glFormat.setVersion(3, 3);
        glFormat.setProfile(QSurfaceFormat::CoreProfile);
    } else {
        glFormat.setVersion(3, 0);
    }

    QSurfaceFormat::setDefaultFormat(glFormat);

    RenderView w;
    w.show();
    return a.exec();
}
