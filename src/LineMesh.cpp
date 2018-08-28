#include "LineMesh.hpp"
#include <array>

namespace ShapeEditor {

LineMesh::LineMesh() {
    initializeOpenGLFunctions();

    glGenBuffers(1, &_vertexBuffer);
    glGenBuffers(1, &_indexBuffer);
    glGenVertexArrays(1, &_vertexArray);

    glBindVertexArray(_vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

LineMesh::~LineMesh() {
    glDeleteVertexArrays(1, &_vertexArray);
    glDeleteBuffers(1, &_vertexBuffer);
    glDeleteBuffers(1, &_indexBuffer);
}

void LineMesh::draw() {
    glBindVertexArray(_vertexArray);
    glDrawElements(GL_LINES, _lineCount * 2, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}

void LineMesh::setLines(const std::vector<std::array<uint16_t, 2>> &lines) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(lines.size() * sizeof(std::vector<std::array<uint16_t, 2>>)), lines.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    _lineCount = int(lines.size());
}

void LineMesh::setVertices(const std::vector<LineMesh::Vertex> &vertices) {
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(Vertex)), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
