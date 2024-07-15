#include "Buffers.h"
#include "Utils.h"
// Usage: setupBuffers(VAO, VBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices))
// No EBO: set indices to nullptr and indicesSize to 0
// WITH EBO
//void setupOldBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO,
//                  const float* vertices, size_t verticesSize,
//                  const unsigned int* indices, size_t indicesSize) {
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
//
//    // Assuming each vertex has 6 floats: 3 for position and 3 for color
//    // Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // Color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//    glBindVertexArray(0);
//}

void setupBuffers(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, const float* vertices,
                  size_t verticesSize, const unsigned int* indices, size_t indicesSize) {
    GL_CHECK(glGenVertexArrays(1, &VAO));
    GL_CHECK(glGenBuffers(1, &VBO));
    GL_CHECK(glGenBuffers(1, &EBO));

    GL_CHECK(glBindVertexArray(VAO));

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW));

    GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW));

    // Position attribute
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)nullptr));
    GL_CHECK(glEnableVertexAttribArray(0));
    // Normal attribute
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(1));
    // Texture Coord attribute
    GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(2));
    // Color tint
    GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(3));

    GL_CHECK(glBindVertexArray(0));
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
//
//    // Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)nullptr);
//    glEnableVertexAttribArray(0);
//    // Normal attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//    // Texture Coord attribute
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
//    glEnableVertexAttribArray(2);
//    // Color tint
//    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
//    glEnableVertexAttribArray(3);
//
//    glBindVertexArray(0);
}


// OVERLOAD: ONLY TEXTURES
void setupBuffersTexturesOnly(unsigned int &VAO, unsigned int &VBO, unsigned int &EBO, const float* vertices,
                  size_t verticesSize, const unsigned int* indices, size_t indicesSize) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Texture Coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}
