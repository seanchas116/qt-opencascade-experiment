#pragma once
#include <QOpenGLExtraFunctions>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class LineMesh final : protected QOpenGLExtraFunctions {
    Q_DISABLE_COPY(LineMesh)
public:
    LineMesh();
    ~LineMesh();

    void draw();
    virtual void setLines(const std::vector<std::array<uint16_t, 2>>& lines);
    virtual void setPositions(const std::vector<glm::vec3>& positions);

private:
    GLuint _vertexArray = 0;
    GLuint _vertexBuffer = 0;
    GLuint _indexBuffer = 0;
    int _lineCount = 0;
};
