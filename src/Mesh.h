#pragma once
#include <QOpenGLExtraFunctions>

class Mesh final : protected QOpenGLExtraFunctions {
public:
    Mesh();
    ~Mesh();

    void draw();
    virtual void setIndices(const std::vector<std::array<uint16_t, 3>>& indices);
    virtual void setVertices(const std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& texCoords);

private:
    GLuint _vertexArray = 0;
    GLuint _vertexBuffer = 0;
    GLuint _indexBuffer = 0;
    int _triangleCount = 0;
};
