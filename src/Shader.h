#pragma once
#include <QOpenGLExtraFunctions>
#include <glm/glm.hpp>

class Shader final : protected QOpenGLExtraFunctions {
    Q_DISABLE_COPY(Shader)
public:
    Shader(const QByteArray& vertexShader, const QByteArray& fragmentShader);
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

    static std::shared_ptr<Shader> fromFiles(const QString& vertexShaderPath, const QString& fragmentShaderPath);

private:
    GLuint loadShader(GLenum type, const QByteArray& src);
    GLuint _program;
};
