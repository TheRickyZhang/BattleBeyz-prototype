#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <iostream>

class ShaderProgram {
public:
    GLuint ID;
    GLint modelLoc, viewLoc, projectionLoc;

    ShaderProgram(const char* vertexSource, const char* fragmentSource);
    void use() const;
    void setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const;
    ~ShaderProgram();

    // Utility uniform functions
    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const;
    void setUniformVec3(const std::string &name, const glm::vec3 &value) const;
    void setUniform1i(const std::string &name, int value) const;

private:
    static GLuint compileShader(GLenum type, const char* source);
};