#pragma once
#include <string>

class Texture {
public:
    unsigned int ID{};
    std::string type;
    std::string path;

    // Constructor for loading and creating a texture
    Texture(const char* imagePath, std::string texType);

    // Method to bind the texture before drawing
    void use() const;
};