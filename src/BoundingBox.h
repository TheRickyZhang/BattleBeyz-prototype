#pragma once

#include <glm/glm.hpp>

class BoundingBox {
public:
    glm::vec3 min;
    glm::vec3 max;

    BoundingBox() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {}
    BoundingBox(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

    bool checkCollision(const BoundingBox& other) const;
};