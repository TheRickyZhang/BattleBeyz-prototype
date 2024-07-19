#include <iostream>
#include "PhysicsWorld.h"

void PhysicsWorld::addBody(RigidBody* body) {
    bodies.push_back(body);
}

void PhysicsWorld::update(float deltaTime) {
    // Update all bodies
    for (RigidBody* body : bodies) {
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

            for (const auto& boxA : bodyA->boundingBoxes) {
                for (const auto& boxB : bodyB->boundingBoxes) {
                    if (boxA->checkCollision(*boxB)) {
                        resolveCollision(bodyA, bodyB);
                    }
                }
            }
        }
    }
}

void PhysicsWorld::resolveCollision(RigidBody* bodyA, RigidBody* bodyB) {
    // Simple collision resolution logic
    glm::vec3 relativeVelocity = bodyB->velocity - bodyA->velocity;
    glm::vec3 collisionNormal = glm::normalize(bodyB->position - bodyA->position);
    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);

    if (velocityAlongNormal > 0) {
        return;
    }

    float restitution = 0.5f; // Coefficient of restitution
    float j = -(1 + restitution) * velocityAlongNormal;
    j /= 1 / bodyA->mass + 1 / bodyB->mass;

    glm::vec3 impulse = j * collisionNormal;
    bodyA->velocity -= impulse / bodyA->mass;
    bodyB->velocity += impulse / bodyB->mass;
}

void PhysicsWorld::renderDebug(ShaderProgram &shader) const {
    // Render all bounding boxes
    for (RigidBody* body : bodies) {
        if(body->boundingBoxes.size() < 100) {
            for (const auto box : body->boundingBoxes) {
                box->renderDebug(shader);
            }
        } else {
            for(int i = 0; i < 100; ++i) {
                body->boundingBoxes[i]->renderDebug(shader);
            }
        }
    }
}
