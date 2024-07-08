#include "RigidBody.h"

RigidBody::RigidBody(const glm::vec3& pos, const glm::vec3& sz, float m)
        : position(pos), size(sz), mass(m), velocity(0.0f), acceleration(0.0f), force(0.0f),
          angularVelocity(0.0f), rotationAxis(0.0f, 1.0f, 0.0f), tiltAngle(0.0f) {}

void RigidBody::applyForce(const glm::vec3& f) {
    force += f;
}

void RigidBody::applyTorque(const glm::vec3& torque) {
    // Update angular velocity based on torque
    // This is a simplified version and does not consider moment of inertia
    angularVelocity += torque / mass;  // Simplified; typically inertia tensor would be used
}

void RigidBody::update(float deltaTime) {
    // Linear dynamics
    acceleration = force / mass;
    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
    force = glm::vec3(0.0f);  // Reset force

    // Angular dynamics
    float angle = glm::length(angularVelocity) * deltaTime;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, rotationAxis);
    rotationAxis = glm::vec3(rotation * glm::vec4(rotationAxis, 1.0f)); // Update rotation axis

    // Update tilt angle if necessary (simplified)
    tiltAngle += angularVelocity.y * deltaTime;  // Assuming tilt change is related to angular velocity in y-axis
}
