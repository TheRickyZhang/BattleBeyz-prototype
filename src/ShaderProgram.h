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
    GLint ambientLoc, diffuseLoc, specularLoc, shininessLoc, dissolveLoc, useTextureLoc, colorLoc;

    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    ~ShaderProgram();

    void use() const;
    void updateCameraPosition(const glm::vec3& cameraPosition, const glm::mat4& viewMatrix) const;
    void setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const;
    void setMaterialUniforms(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, float dissolve, bool useTexture, const glm::vec3& color) const;

    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const;
    void setUniformVec3(const std::string &name, const glm::vec3 &value) const;
    void setUniform1f(const std::string &name, float value) const;
    void setInt(const std::string &name, int value) const;
    void setBool(const std::string &name, bool value) const;

private:
    static std::string readFile(const char* filePath);
    GLuint compileShader(GLenum type, const char* source);
    [[nodiscard]] GLint getUniformLocation(const std::string &name) const;
    void initializeUniformLocations();
    bool isUniformAvailable(const std::string& name) const;
};

#endif // SHADER_PROGRAM_H
