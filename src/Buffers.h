#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Takes in Vertex Array Object, Vertex Buffer Object, Element Buffer Object, vertices, and indices
// Binds the objects such that the vertex attributes are stored as 3 pos, 3 color floats
void setupBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
                  const float* vertices, size_t verticesSize,
                  const unsigned int* indices, size_t indicesSize);

// Overloaded version with no EBO
void setupBuffers(unsigned int &VAO, unsigned int &VBO, const float* vertices, size_t verticesSize);