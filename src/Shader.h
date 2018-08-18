#pragma once
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>

class Shader final : protected QOpenGLExtraFunctions {
public:
    Shader(const QString& vertexShader, const QString& fragmentShader);
    ~Shader();

    void bind();
    void unbind();

    void setUniform(const char* name, float value);
    void setUniform(const char* name, glm::vec2 value);
    void setUniform(const char* name, glm::vec3 value);
    void setUniform(const char* name, glm::vec4 value);
    void setUniform(const char* name, glm::mat2 value);
    void setUniform(const char* name, glm::mat3 value);
    void setUniform(const char* name, glm::mat4 value);
};
