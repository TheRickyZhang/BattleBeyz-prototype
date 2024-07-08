#pragma once

#include <vector>
#include "RigidBody.h"

class PhysicsWorld {
public:
    std::vector<RigidBody*> bodies;

    void addBody(RigidBody* body);
    void update(float deltaTime);
private:
    void detectCollisions();
    static void resolveCollision(RigidBody* bodyA, RigidBody* bodyB);
};
