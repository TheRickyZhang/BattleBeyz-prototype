#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "ShaderProgram.h"
#include "Utils.h"

class TextRenderer {
public:
    TextRenderer(const char* fontPath, unsigned int VAO, unsigned int VBO);
    ~TextRenderer();
    void RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color);

    ShaderProgram * getShaderProgram();

private:
    FT_Library ft{};
    FT_Face face{};
    GLuint textureID{};
    GLuint VAO, VBO;
    ShaderProgram* shaderProgram{}; // Shader program for rendering text

    void initRenderData();
};
