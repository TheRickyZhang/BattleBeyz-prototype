#include "Stadium.h"
#include <cmath>
#include <algorithm>

Stadium::Stadium(unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3& pos, const glm::vec3& col,
                 float radius, float curvature, float meshQuality)
        : GameObject(vao, vbo, ebo, pos, col), radius(radius), curvature(curvature), meshQuality(meshQuality) {
    Stadium::initializeMesh();
}

// Handles generation of vertices, normals, indices, tangents, and texture coordinates
void Stadium::generateMeshData() {
    float targetLinearDistance = radius / static_cast<float>(meshQuality);
    for (int rIdx = 0; rIdx <= static_cast<int>(meshQuality); ++rIdx) {
        float r = static_cast<float>(rIdx) / static_cast<float>(meshQuality) * radius;
        int thetaSegments = std::max(1, static_cast<int>(2 * M_PI * r / targetLinearDistance));

        for (int thetaIdx = 0; thetaIdx <= thetaSegments; ++thetaIdx) {
            float theta = glm::radians(static_cast<float>(thetaIdx) / static_cast<float>(thetaSegments) * 360.0f);
            float x = r * std::cos(theta);
            float y = r * std::sin(theta);
            float z = (std::pow(x, 2.0f) / curvature) + (std::pow(y, 2.0f) / curvature);

            vertices.emplace_back(x, y, z);
            normals.emplace_back(0.0f, 0.0f, 0.0f); // Initialize normals to zero
            texCoords.emplace_back(static_cast<float>(thetaIdx) / static_cast<float>(thetaSegments), static_cast<float>(rIdx) / static_cast<float>(meshQuality));
        }
    }

    // Calculate the normal for each triangle and add it to each vertex normal
    for (int rIdx = 0; rIdx <= static_cast<int>(meshQuality); ++rIdx) {
        int currentRingVertexCount = std::max(1, static_cast<int>(2 * M_PI * (static_cast<float>(rIdx) / static_cast<float>(meshQuality) * radius)));
        int nextRingVertexCount = std::max(1, static_cast<int>(2 * M_PI * (static_cast<float>(rIdx + 1) / static_cast<float>(meshQuality) * radius)));
        int totalVertices = rIdx * currentRingVertexCount;

        for (int thetaIdx = 0; thetaIdx < currentRingVertexCount; ++thetaIdx) {
            int current = totalVertices + thetaIdx;
            int next = totalVertices + (thetaIdx + 1) % currentRingVertexCount;
            int nextRingCurrent = totalVertices + currentRingVertexCount + thetaIdx % nextRingVertexCount;
            int nextRingNext = totalVertices + currentRingVertexCount + (thetaIdx + 1) % nextRingVertexCount;

            glm::vec3 v0 = vertices[current];
            glm::vec3 v1 = vertices[nextRingCurrent];
            glm::vec3 v2 = vertices[next];

            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            normals[current] += normal;
            normals[nextRingCurrent] += normal;
            normals[next] += normal;

            // Triangle 1
            indices.push_back(current);
            indices.push_back(nextRingCurrent);
            indices.push_back(next);

            // Second triangle vertices
            v0 = vertices[next];
            v1 = vertices[nextRingCurrent];
            v2 = vertices[nextRingNext];

            edge1 = v1 - v0;
            edge2 = v2 - v0;
            normal = glm::normalize(glm::cross(edge1, edge2));

            normals[next] += normal;
            normals[nextRingCurrent] += normal;
            normals[nextRingNext] += normal;

            // Triangle 2
            indices.push_back(next);
            indices.push_back(nextRingCurrent);
            indices.push_back(nextRingNext);
        }
    }

    // Normalize the normals
    for (auto& normal : normals) {
        normal = glm::normalize(normal);
    }
}

void Stadium::initializeMesh() {
    generateMeshData();
    setupBuffers();
}

void Stadium::render(ShaderProgram& shader) {
    shader.use();

    // Bind appropriate uniforms (model, view, projection matrices)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setUniformMat4("model", model);
    shader.setUniformVec3("objectColor", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
