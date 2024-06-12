#include "ShaderSource.h"
#include "Utils.h"

// Load shader source from files
std::string vertexShaderSourceStr;
std::string fragmentShaderSourceStr;
std::string textVertexShaderSourceStr;
std::string textFragmentShaderSourceStr;

const char* GetVertexShaderSource() {
    vertexShaderSourceStr = readFile("../assets/shaders/vertexShader.vs");
    return vertexShaderSourceStr.data();
}

const char* GetFragmentShaderSource() {
    fragmentShaderSourceStr = readFile("../assets/shaders/fragmentShader.fs");
    return fragmentShaderSourceStr.data();
}

const char* GetTextVertexShaderSource() {
    textVertexShaderSourceStr = readFile("../assets/shaders/textVertexShader.vs");
    return textVertexShaderSourceStr.data();
}

const char* GetTextFragmentShaderSource() {
    textFragmentShaderSourceStr = readFile("../assets/shaders/textFragmentShader.fs");
    return textFragmentShaderSourceStr.data();
}
