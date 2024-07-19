#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "RigidBody.h"
#include "ShaderProgram.h"

class PhysicsWorld {
public:
    std::vector<RigidBody*> bodies;

    void addBody(RigidBody* body);
    void update(float deltaTime);
    void renderDebug(ShaderProgram &shader) const;

private:
    void detectCollisions();
    static void resolveCollision(RigidBody* bodyA, RigidBody* bodyB);
};
