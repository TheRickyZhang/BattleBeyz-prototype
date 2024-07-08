#include "PhysicsWorld.h"

void PhysicsWorld::addBody(RigidBody* body) {
    bodies.push_back(body);
}

void PhysicsWorld::update(float deltaTime) {
    // Update all bodies
    for (auto body : bodies) {
        body->update(deltaTime);
    }

    // Detect and resolve collisions
    detectCollisions();
}

void PhysicsWorld::detectCollisions() {
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            RigidBody* bodyA = bodies[i];
            RigidBody* bodyB = bodies[j];

            // Simple AABB collision detection
            if (glm::abs(bodyA->position.x - bodyB->position.x) < (bodyA->size.x + bodyB->size.x) &&
                glm::abs(bodyA->position.y - bodyB->position.y) < (bodyA->size.y + bodyB->size.y) &&
                glm::abs(bodyA->position.z - bodyB->position.z) < (bodyA->size.z + bodyB->size.z)) {
                resolveCollision(bodyA, bodyB);
            }
        }
    }
}

void PhysicsWorld::resolveCollision(RigidBody* bodyA, RigidBody* bodyB) {
    // Simple collision resolution by reversing velocity (for demonstration purposes)
    bodyA->velocity = -bodyA->velocity;
    bodyB->velocity = -bodyB->velocity;
}