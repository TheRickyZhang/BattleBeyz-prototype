#include <iostream>
#include "BoundingBox.h"

BoundingBox::BoundingBox()
        : min(glm::vec3(FLT_MAX)), max(glm::vec3(-FLT_MAX)), VAO(0), VBO(0), EBO(0) {
    setupBuffers();
}

BoundingBox::BoundingBox(const glm::vec3& min, const glm::vec3& max)
        : min(min), max(max), VAO(0), VBO(0), EBO(0) {
    setupBuffers();
}

BoundingBox::~BoundingBox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool BoundingBox::checkCollision(const BoundingBox &other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

bool BoundingBox::intersectsSphere(const glm::vec3& center, float radius) const {
    float dist_squared = radius * radius;

    if (center.x < min.x) dist_squared -= (center.x - min.x) * (center.x - min.x);
    else if (center.x > max.x) dist_squared -= (center.x - max.x) * (center.x - max.x);

    if (center.y < min.y) dist_squared -= (center.y - min.y) * (center.y - min.y);
    else if (center.y > max.y) dist_squared -= (center.y - max.y) * (center.y - max.y);

    if (center.z < min.z) dist_squared -= (center.z - min.z) * (center.z - min.z);
    else if (center.z > max.z) dist_squared -= (center.z - max.z) * (center.z - max.z);

    return dist_squared > 0;
}

void BoundingBox::update(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
    min = glm::min(v1, glm::min(v2, v3));
    max = glm::max(v1, glm::max(v2, v3));
}

void BoundingBox::update(const glm::vec3& position, const glm::quat& orientation) {
    glm::vec3 halfSize = (max - min) * 0.5f;
    glm::vec3 center = min + halfSize;

    // Rotate the center and halfSize using the quaternion
    glm::vec3 rotatedCenter = glm::rotate(orientation, center);
    glm::vec3 rotatedHalfSize = glm::abs(glm::rotate(orientation, halfSize));

    glm::vec3 newCenter = position + rotatedCenter;

    min = newCenter - rotatedHalfSize;
    max = newCenter + rotatedHalfSize;
}

void BoundingBox::expandToInclude(const BoundingBox& other) {
    min = glm::min(min, other.min);
    max = glm::max(max, other.max);
}

void BoundingBox::expandToInclude(const glm::vec3& point) {
    min = glm::min(min, point);
    max = glm::max(max, point);
}

void BoundingBox::setupBuffers() {
    float vertices[] = {
            // Positions         // Normals         // Texture Coords  // Colors (RGB)
            min.x, min.y, min.z,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,      1.0f, 0.0f, 0.0f, // Red
            max.x, min.y, min.z,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,      0.0f, 1.0f, 0.0f, // Green
            max.x, max.y, min.z,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,      0.0f, 0.0f, 1.0f, // Blue
            min.x, max.y, min.z, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,      1.0f, 1.0f, 0.0f, // Yellow
            min.x, min.y, max.z,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,      1.0f, 0.0f, 1.0f, // Magenta
            max.x, min.y, max.z,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,      0.0f, 1.0f, 1.0f, // Cyan
            max.x, max.y, max.z,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,      1.0f, 0.5f, 0.0f, // Orange
            min.x, max.y, max.z, -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,      0.5f, 0.5f, 0.5f  // Grey
    };

    unsigned int indices[] = {
            0, 1, 2, 2, 3, 0, // Bottom face
            4, 5, 6, 6, 7, 4, // Top face
            0, 1, 5, 5, 4, 0, // Front face
            1, 2, 6, 6, 5, 1, // Right face
            2, 3, 7, 7, 6, 2, // Back face
            3, 0, 4, 4, 7, 3  // Left face
    };

    ::setupBuffers(VAO, VBO, EBO, vertices, sizeof(vertices), indices, sizeof(indices));
}

void BoundingBox::renderDebug(ShaderProgram &shader, const glm::vec3 &viewPos) {
    setupBuffers();
    std::cout << "Rendering bounding box" << min.x << " " << min.y << " " << min.z << "\n" << max.x << " " << max.y << " " << max.z << "\n" << std::endl;
    shader.use();

    shader.setUniformVec3("viewPos", viewPos); // Example view position
    shader.setUniformVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr); // 36 is the number of indices for 12 triangles
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
}