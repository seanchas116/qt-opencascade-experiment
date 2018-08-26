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
#include <GeomLProp_SLProps.hxx>
#include "MakeBottle.hpp"
#include "LineMesh.hpp"

RenderView::RenderView(QWidget* parent) : QOpenGLWidget(parent) {
    setFocus();

    updateCameraMatrix();
}

void RenderView::initializeGL() {
    initializeOpenGLFunctions();

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    _shader = Shader::fromFiles(":/Face.vert", ":/Face.frag");
    _mesh = std::make_shared<Mesh>();
    _edgeMesh = std::make_shared<LineMesh>();
    _edgeShader = Shader::fromFiles(":/Edge.vert", ":/Edge.frag");

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

        const TColgp_Array1OfPnt& nodes = triangulation->Nodes();

        int indexOffset = vertices.size() - nodes.Lower();

        for (int i = 0; i < nodes.Length(); ++i) {
            gp_Pnt p = nodes.Value(nodes.Lower() + i);
            glm::vec3 pos(p.X(), p.Y(), p.Z());

            gp_Pnt2d uv = triangulation->UVNodes()(triangulation->UVNodes().Lower() + i);

            auto surface = BRep_Tool::Surface(face);
            GeomLProp_SLProps props(surface, uv.X(), uv.Y(), 1, 0.01);
            auto normal = props.Normal();
            glm::vec3 n(normal.X(), normal.Y(), normal.Z());
            if (face.Orientation() == TopAbs_REVERSED) {
                n = -n;
            }
            vertices.push_back({pos, {0, 0}, n});
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

    std::vector<glm::vec3> linePositions;
    std::vector<std::array<uint16_t, 2>> lines;

    TopExp_Explorer edgeExplorer(shape, TopAbs_EDGE);
    for (; edgeExplorer.More(); edgeExplorer.Next()) {
        auto edge = TopoDS::Edge(edgeExplorer.Current());
        Handle(Poly_PolygonOnTriangulation) polygon;
        Handle(Poly_Triangulation) triangulation;
        TopLoc_Location location;
        BRep_Tool::PolygonOnTriangulation(edge, polygon, triangulation, location);

        bool first = true;
        for (auto i : polygon->Nodes()) {
            auto p = triangulation->Nodes().Value(i);
            glm::vec3 pos(p.X(), p.Y(), p.Z());
            linePositions.push_back(pos);
            if (!first) {
                uint16_t i1 = linePositions.size() - 2;
                uint16_t i2 = linePositions.size() - 1;
                lines.push_back({i1, i2});
            }
            first = false;
        }
    }

    _edgeMesh->setLines(lines);
    _edgeMesh->setPositions(linePositions);
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
    _edgeShader->setUniform("MVP", _projectionMatrix * _cameraMatrix);

    _shader->bind();
    _mesh->draw();

    _edgeShader->bind();
    _edgeMesh->draw();
}

void RenderView::keyPressEvent(QKeyEvent *event) {
    _pressedKeys.insert(event->key());
}

void RenderView::keyReleaseEvent(QKeyEvent *event) {
    _pressedKeys.remove(event->key());
}

void RenderView::mousePressEvent(QMouseEvent *event) {
    if (_pressedKeys.contains(Qt::Key_Space) || event->button() == Qt::MiddleButton) {
        _dragMode = DragMode::Move;
    } else if (event->button() == Qt::RightButton) {
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
