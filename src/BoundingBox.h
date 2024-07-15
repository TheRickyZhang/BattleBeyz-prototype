#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp> // For glm::rotate
#include "ShaderProgram.h"
#include "Buffers.h"

class BoundingBox {
public:
    glm::vec3 min;
    glm::vec3 max;

    GLuint VAO{}, VBO{}, EBO{};

    BoundingBox();
    BoundingBox(const glm::vec3& min, const glm::vec3& max);
    ~BoundingBox();

    [[nodiscard]] bool checkCollision(const BoundingBox& other) const;
    [[nodiscard]] bool intersectsSphere(const glm::vec3& center, float radius) const;
    void update(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3);
    void update(const glm::vec3& position, const glm::quat& orientation);
    void expandToInclude(const BoundingBox& other);
    void expandToInclude(const glm::vec3& point);
    void renderDebug(ShaderProgram &shader, const glm::vec3 &viewPos);
private:
    void setupBuffers();

};