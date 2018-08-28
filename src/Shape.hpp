#pragma once

#include <TopoDS.hxx>
#include <memory>
#include <glm/glm.hpp>

namespace ShapeEditor {

class Shader;
class Mesh;
class LineMesh;

class Shape {
public:
    Shape();

    void setOCCShape(const TopoDS_Shape& shape);

    void render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
    static const std::shared_ptr<Shader> &sharedShader();
    static const std::shared_ptr<Shader> &sharedEdgeShader();

    std::shared_ptr<Mesh> _mesh;
    std::shared_ptr<LineMesh> _edgeMesh;
};

} // namespace ShapeEditor
