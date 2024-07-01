#include <GL/glew.h>
#include "Texture.h"
#include <iostream>
#include <utility>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* imagePath, std::string texType) : type(std::move(texType)), path(imagePath) {
    std::cout << "Loading texture: " << imagePath << std::endl; // Add this line

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set texture wrapping and filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image data
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  // Ensure texture coordinates match
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);  // Generate mipmaps for the texture
        std::cout << "Loaded texture: " << imagePath << " with dimensions: " << width << "x" << height << " and channels: " << nrChannels << std::endl;
    } else {
        std::cout << "Failed to load texture: " << imagePath << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done to prevent accidental modification
}


void Texture::use() const {
    glBindTexture(GL_TEXTURE_2D, ID);
}
