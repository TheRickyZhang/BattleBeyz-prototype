#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class ShaderProgram {
public:
    GLuint ID;
    GLint modelLoc, viewLoc, projectionLoc;

    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();
    void use() const;
    void setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const;
    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const;
    void setUniformVec3(const std::string &name, const glm::vec3 &value) const;
    void setUniform1i(const std::string &name, int value) const;

private:
    std::string readFile(const char* filePath) const;
    GLuint compileShader(GLenum type, const char* source);
};

#endif