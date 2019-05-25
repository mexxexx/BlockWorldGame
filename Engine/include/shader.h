#ifndef __ENGINE_SHADER_H__
#define __ENGINE_SHADER_H__

#include <string>

#include "glad/glad.h"
#include "glm/glm.hpp"

namespace engine
{
class Shader
{
public:
    Shader();
    ~Shader();
    GLuint getglShaderProgram();
    int loadShader(const std::string vertexPath, const std::string geometryPath, const std::string fragmentPath);
    void setBool(const std::string &name, GLboolean value) const;
    void setInt(const std::string &name, GLint value) const;
    void setFloat(const std::string &name, GLfloat value) const;
    void setMatrix4fv(const std::string &name, glm::mat4 value) const;
    void use();
    void deleteShader();

private:
    GLuint glShaderProgram;
    int checkCompileErrors(GLuint shader, std::string type);
    GLuint loadAndCompileShader(const std::string path, const std::string name, const GLenum shaderType);
};
} // namespace engine

#endif