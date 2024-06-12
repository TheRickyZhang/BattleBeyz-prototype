#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "ShaderProgram.h"

struct Character {
    GLuint TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;   // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;    // Offset to advance to next glyph
};

class TextRenderer {
public:
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    ShaderProgram shader;
    FT_Library ft; // Declare FT_Library

    TextRenderer(const char* font, GLuint width, GLuint height);
    void Load(const char* font, GLuint fontSize);
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};
