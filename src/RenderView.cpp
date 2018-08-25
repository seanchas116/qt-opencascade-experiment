#include "RenderView.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <QKeyEvent>
#include <QtDebug>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopLoc_Location.hxx>
#include <Poly.hxx>
#include "MakeBottle.hpp"

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();

    updateCameraMatrix();
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    _shader = Shader::fromFiles(":/Default.vert", ":/Default.frag");
    _mesh = std::make_shared<Mesh>();

//    std::vector<Vertex> vertices = {
//        {{0, -1, 0}, {0, 0}, {0, 0, 1}},
//        {{-1, 1, 0}, {0, 0}, {0, 0, 1}},
//        {{1, 1, 0}, {0, 0}, {0, 0, 1}},
//    };
//    std::vector<std::array<uint16_t, 3>> triangles = {
//        {{0, 1, 2}}
//    };
//    _mesh->setTriangles(triangles);
//    _mesh->setVertices(vertices);

    auto shape = MakeBottle(10, 10, 4);

    BRepMesh_IncrementalMesh meshing(shape, 0.01, false, 0.5);
    meshing.Perform();

    TopExp_Explorer explorer(shape, TopAbs_FACE);

    std::vector<Vertex> vertices;
    std::vector<std::array<uint16_t, 3>> triangleIndexes;

    for (; explorer.More(); explorer.Next()) {
        TopoDS_Face face = TopoDS::Face(explorer.Current());
        TopLoc_Location location;
        auto triangulation = BRep_Tool::Triangulation(face, location);
        Poly::ComputeNormals(triangulation);

        const TColgp_Array1OfPnt& nodes = triangulation->Nodes();
        auto& normals = triangulation->Normals();

        int indexOffset = vertices.size() - nodes.Lower();

        for (int i = 0; i < nodes.Length(); ++i) {
            gp_Pnt p = nodes.Value(nodes.Lower() + i);
            glm::vec3 pos(p.X(), p.Y(), p.Z());
            int normalIndex = normals.Lower() + i * 3;
            glm::vec3 normal(normals.Value(normalIndex), normals.Value(normalIndex + 1), normals.Value(normalIndex + 2));
            vertices.push_back({pos, {0, 0}, normal});
        }
        const Poly_Array1OfTriangle& triangles = triangulation->Triangles();
        for (int i = triangles.Lower() ; i <= triangles.Upper(); ++i) {
            Poly_Triangle t = triangles.Value(i);
            uint16_t t1 = t.Value(1) + indexOffset;
            uint16_t t2 = t.Value(2) + indexOffset;
            uint16_t t3 = t.Value(3) + indexOffset;
            triangleIndexes.push_back({{t1, t2, t3}});
        }
    }

    _mesh->setTriangles(triangleIndexes);
    _mesh->setVertices(vertices);
}

void RenderView::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    _projectionMatrix = glm::perspective(glm::radians(90.f), float(width()) / float(height()), 0.1f, 100.f);
}

void RenderView::paintGL() {
    glClearColor(0.6f, 0.6f, 0.6f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader->setUniform("MVP", _projectionMatrix * _cameraMatrix);
    _shader->setUniform("MV", _cameraMatrix);

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
    updateCameraMatrix();
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
