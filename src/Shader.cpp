#include "Shader.h"
#include <QtDebug>

Shader::Shader(const std::string &vertexShader, const std::string &fragmentShader) {
    initializeOpenGLFunctions();

    GLuint program = glCreateProgram();
    GLuint vertex = loadShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fragment = loadShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked) {
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);

        std::vector<char> log(size_t(logLen), 0);
        glGetProgramInfoLog(program, logLen, nullptr, log.data());
        qWarning() << log.data();

        glDeleteProgram(_program);
    }
    _program = program;
}

Shader::~Shader() {
    glDeleteProgram(_program);
}

void Shader::bind() {
    glUseProgram(_program);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::setUniform(const char *name, float value) {
    glUseProgram(_program);
    glUniform1f(glGetUniformLocation(_program, name), value);
}

void Shader::setUniform(const char *name, glm::vec2 value) {
    glUseProgram(_program);
    glUniform2f(glGetUniformLocation(_program, name), value.x, value.y);
}

void Shader::setUniform(const char *name, glm::vec3 value) {
    glUseProgram(_program);
    glUniform3f(glGetUniformLocation(_program, name), value.x, value.y, value.z);
}

void Shader::setUniform(const char *name, glm::vec4 value) {
    glUseProgram(_program);
    glUniform4f(glGetUniformLocation(_program, name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const char *name, glm::mat2 value) {
    glUseProgram(_program);
    glUniformMatrix2fv(glGetUniformLocation(_program, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const char *name, glm::mat3 value) {
    glUseProgram(_program);
    glUniformMatrix3fv(glGetUniformLocation(_program, name), 1, GL_FALSE, &value[0][0]);
}

void Shader::setUniform(const char *name, glm::mat4 value) {
    glUseProgram(_program);
    glUniformMatrix4fv(glGetUniformLocation(_program, name), 1, GL_FALSE, &value[0][0]);
}

GLuint Shader::loadShader(GLenum type, const std::string &src) {
    GLuint shader = glCreateShader(type);
    const char* srcData = src.data();
    glShaderSource(shader, 1, &srcData, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

    if (!compiled) {
        GLint logLen;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);

        std::vector<char> log(size_t(logLen), 0);
        glGetShaderInfoLog(shader, logLen, nullptr, log.data());
        qWarning() << log.data();

        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
