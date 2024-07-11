#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Constructor
ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath) {
    // Read shader source files
    std::string vertexCode = readFile(vertexPath);
    std::string fragmentCode = readFile(fragmentPath);

    // Compile and link shaders
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexCode.c_str());
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Check for linking errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Initialize uniform locations
    modelLoc = glGetUniformLocation(ID, "model");
    viewLoc = glGetUniformLocation(ID, "view");
    projectionLoc = glGetUniformLocation(ID, "projection");
    initializeUniformLocations();
}

bool ShaderProgram::isUniformAvailable(const std::string& name) const {
    GLint location = glGetUniformLocation(ID, name.c_str());
    return location != -1;
}

// Initialize uniform locations
void ShaderProgram::initializeUniformLocations() {
    ambientLoc = glGetUniformLocation(ID, "material.ambient");
    diffuseLoc = glGetUniformLocation(ID, "material.diffuse");
    specularLoc = glGetUniformLocation(ID, "material.specular");
    shininessLoc = glGetUniformLocation(ID, "material.shininess");
    dissolveLoc = glGetUniformLocation(ID, "material.dissolve");
    useTextureLoc = glGetUniformLocation(ID, "material.useTexture");
    colorLoc = glGetUniformLocation(ID, "material.color");

    std::cout << "Uniform locations: " << std::endl;
    std::cout << "ambientLoc: " << ambientLoc << std::endl;
    std::cout << "diffuseLoc: " << diffuseLoc << std::endl;
    std::cout << "specularLoc: " << specularLoc << std::endl;
    std::cout << "shininessLoc: " << shininessLoc << std::endl;
    std::cout << "dissolveLoc: " << dissolveLoc << std::endl;
    std::cout << "useTextureLoc: " << useTextureLoc << std::endl;
    std::cout << "colorLoc: " << colorLoc << std::endl;
}

// Destructor
ShaderProgram::~ShaderProgram() {
    glDeleteProgram(ID);
}

// Use the current program
void ShaderProgram::use() const {
    glUseProgram(ID);
}

// Update camera position and view matrix
void ShaderProgram::updateCameraPosition(const glm::vec3& cameraPosition, const glm::mat4& viewMatrix) const {
    setUniformVec3("viewPos", cameraPosition);
    setUniformMat4("view", viewMatrix);
}

// Set uniforms for model, view, and projection matrices
void ShaderProgram::setUniforms(glm::mat4 model, glm::mat4 view, glm::mat4 projection) const {
    use(); // Activate the shader program
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void ShaderProgram::setMaterialUniforms(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shininess, float dissolve, bool useTexture, const glm::vec3& color) const {
    if (isUniformAvailable("material.ambient")) {
        glUniform3fv(ambientLoc, 1, glm::value_ptr(ambient));
    }
    if (isUniformAvailable("material.diffuse")) {
        glUniform3fv(diffuseLoc, 1, glm::value_ptr(diffuse));
    }
    if (isUniformAvailable("material.specular")) {
        glUniform3fv(specularLoc, 1, glm::value_ptr(specular));
    }
    if (isUniformAvailable("material.shininess")) {
        glUniform1f(shininessLoc, shininess);
    }
    if (isUniformAvailable("material.dissolve")) {
        glUniform1f(dissolveLoc, dissolve);
    }
    if (isUniformAvailable("material.useTexture")) {
        glUniform1i(useTextureLoc, useTexture);
    }
    if (isUniformAvailable("material.color")) {
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    }
}

// Utility uniform functions
void ShaderProgram::setUniformMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void ShaderProgram::setUniformVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniform1f(const std::string &name, float value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1f(location, value);
    }
}

void ShaderProgram::setInt(const std::string &name, int value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

void ShaderProgram::setBool(const std::string &name, bool value) const {
    GLint location = getUniformLocation(name);
    if (location != -1) {
        glUniform1i(location, value);
    }
}

// Helper functions
std::string ShaderProgram::readFile(const char* filePath) {
    std::ifstream file(filePath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint ShaderProgram::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLint ShaderProgram::getUniformLocation(const std::string &name) const {
    return glGetUniformLocation(ID, name.c_str());
}
