#include "Shape.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "LineMesh.hpp"
#include <TopExp_Explorer.hxx>
#include <TopoDS_Face.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>
#include <TopLoc_Location.hxx>
#include <Poly.hxx>
#include <GeomLProp_SLProps.hxx>
#include <array>

namespace ShapeEditor {

Shape::Shape() {
    _mesh = std::make_shared<Mesh>();
    _edgeMesh = std::make_shared<LineMesh>();
}

void Shape::setOCCShape(const TopoDS_Shape &shape) {
    BRepMesh_IncrementalMesh meshing(shape, 0.01, false, 0.5);
    meshing.Perform();

    {
        TopExp_Explorer explorer(shape, TopAbs_FACE);

        std::vector<Mesh::Vertex> vertices;
        std::vector<std::array<uint16_t, 3>> triangleIndexes;

        for (; explorer.More(); explorer.Next()) {
            TopoDS_Face face = TopoDS::Face(explorer.Current());
            TopLoc_Location location;
            auto triangulation = BRep_Tool::Triangulation(face, location);

            const TColgp_Array1OfPnt& nodes = triangulation->Nodes();

            int indexOffset = int(vertices.size()) - nodes.Lower();

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
                auto t1 = uint16_t(t.Value(1) + indexOffset);
                auto t2 = uint16_t(t.Value(2) + indexOffset);
                auto t3 = uint16_t(t.Value(3) + indexOffset);
                triangleIndexes.push_back({{t1, t2, t3}});
            }
        }

        _mesh->setTriangles(triangleIndexes);
        _mesh->setVertices(vertices);
    }

    {
        std::vector<LineMesh::Vertex> lineVertices;
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
                lineVertices.push_back({pos});
                if (!first) {
                    auto i1 = uint16_t(lineVertices.size() - 2);
                    auto i2 = uint16_t(lineVertices.size() - 1);
                    lines.push_back({i1, i2});
                }
                first = false;
            }
        }

        _edgeMesh->setLines(lines);
        _edgeMesh->setVertices(lineVertices);
    }
}

void Shape::render(const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix) {
    auto& shader = sharedShader();
    auto& edgeShader = sharedEdgeShader();
    shader->setUniform("MVP", projectionMatrix * viewMatrix);
    shader->setUniform("MV", viewMatrix);
    edgeShader->setUniform("MVP", projectionMatrix * viewMatrix);

    shader->bind();
    _mesh->draw();

    edgeShader->bind();
    _edgeMesh->draw();
}

const std::shared_ptr<Shader> &Shape::sharedShader() {
    static auto shader = Shader::fromFiles(":/Face.vert", ":/Face.frag");
    return shader;
}

const std::shared_ptr<Shader> &Shape::sharedEdgeShader() {
    static auto shader = Shader::fromFiles(":/Edge.vert", ":/Edge.frag");
    return shader;
}

} // namespace ShapeEditor
