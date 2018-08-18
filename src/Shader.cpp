#include "Shader.h"

Shader::Shader(const QString &vertexShader, const QString &fragmentShader) {
    initializeOpenGLFunctions();
}

Shader::~Shader() {

}

void Shader::bind() {

}

void Shader::unbind() {

}

void Shader::setUniform(const char *name, float value) {

}

void Shader::setUniform(const char *name, glm::vec2 value) {

}

void Shader::setUniform(const char *name, glm::vec3 value) {

}

void Shader::setUniform(const char *name, glm::vec4 value) {

}

void Shader::setUniform(const char *name, glm::mat2 value) {

}

void Shader::setUniform(const char *name, glm::mat3 value) {

}

void Shader::setUniform(const char *name, glm::mat4 value) {

}
