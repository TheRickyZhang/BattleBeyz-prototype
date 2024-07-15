#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <memory>
#include "BoundingBox.h"
#include "Utils.h"

class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    std::vector<std::unique_ptr<BoundingBox>> boundingBoxes;
    float mass;
    glm::vec3 force;
    glm::vec3 angularVelocity;  // Angular velocity vector
    glm::vec3 torque;           // Accumulated torque
    glm::quat orientation;      // Orientation quaternion
    glm::mat3 inertiaTensor;    // Inertia tensor in the body frame
    glm::mat3 inverseInertiaTensor; // Inverse inertia tensor in the world frame
    BoundingBox aggregateBoundingBox; // Aggregate bounding box

    RigidBody(const glm::vec3& pos, const glm::vec3& sz, float m, std::vector<std::unique_ptr<BoundingBox>> bboxes = {});
    virtual ~RigidBody(); // Destructor to manage memory

    void applyForce(const glm::vec3& f);
    void applyTorque(const glm::vec3& t);

    virtual void update(float deltaTime);

    void setupBuffers();
    void renderDebug(ShaderProgram &shader, const glm::vec3 &viewPos);

    void updateBoundingBoxes();

private:
    void updateInertiaTensor();

    GLuint VAO, VBO, EBO;
};

class ImmovableRigidBody : public RigidBody {
public:
    ImmovableRigidBody(const glm::vec3& pos, const glm::vec3& sz, std::vector<std::unique_ptr<BoundingBox>> bboxes = {})
            : RigidBody(pos, sz, FLT_MAX, std::move(bboxes)) {} // Infinite mass for immovable body

    void update(float deltaTime) override {
        // No dynamics update for immovable bodies
    }
};
