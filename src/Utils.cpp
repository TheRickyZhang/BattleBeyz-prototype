#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readFile(const char* filePath) {
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filePath << std::endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}