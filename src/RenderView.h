#pragma once
#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>

class Mesh;
class Shader;

class RenderView : public QOpenGLWidget, protected QOpenGLExtraFunctions {
public:
    RenderView(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    std::shared_ptr<Mesh> _mesh;
    std::shared_ptr<Shader> _shader;
};
