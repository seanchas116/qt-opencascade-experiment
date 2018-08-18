#pragma once
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class RenderView : public QOpenGLWidget, protected QOpenGLExtraFunctions {
public:
    RenderView(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
