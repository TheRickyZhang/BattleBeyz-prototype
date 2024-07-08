#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    glm::vec3 size;          // For bounding box
    float mass;
    glm::vec3 force;
    glm::vec3 angularVelocity;  // Angular velocity vector
    glm::vec3 rotationAxis;     // Axis of rotation
    float tiltAngle;            // Tilt angle

    RigidBody(const glm::vec3& pos, const glm::vec3& sz, float m);

    void applyForce(const glm::vec3& f);
    void applyTorque(const glm::vec3& torque);
    void update(float deltaTime);
};