#pragma once
#include <QOpenGLExtraFunctions>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoord;
    glm::vec3 normal;
};

class Mesh final : protected QOpenGLExtraFunctions {
    Q_DISABLE_COPY(Mesh)
public:
    Mesh();
    ~Mesh();

    void draw();
    virtual void setTriangles(const std::vector<std::array<uint16_t, 3>>& triangles);
    virtual void setVertices(const std::vector<Vertex>& vertices);

private:
    GLuint _vertexArray = 0;
    GLuint _vertexBuffer = 0;
    GLuint _indexBuffer = 0;
    int _triangleCount = 0;
};
