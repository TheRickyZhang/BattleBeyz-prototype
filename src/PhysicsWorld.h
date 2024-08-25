////////////////////////////////////////////////////////////////////////////////
// PhysicsWorld.cpp -- Core physics handling include -- rz -- 2024-08-08
// Copyright (c) 2024 Ricky Zhang
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Physics.h"
#include "RigidBodies/StadiumBody.h"
#include "RigidBodies/BeybladeBody.h"

#include <vector>
#include <glm/glm.hpp>
#include <unordered_map>
#include "Utils.h"
#include "RigidBody.h"
#include "ShaderProgram.h"
#include "Physics.h"

class PhysicsWorld {
public:
    PhysicsWorld(double fluidDragValue = 0.8, double spinThreshold = 30) : fluidDrag(fluidDragValue), SPIN_THRESHOLD(spinThreshold) {}

    void addBeybladeBody(BeybladeBody* body);
    void addStadiumBody(StadiumBody* body);
    void removeBeybladeBody(BeybladeBody* body);
    void removeStadiumBody(StadiumBody* body);

    void update(float deltaTime);
    void renderDebug(ShaderProgram &shader) const;
    std::vector<BeybladeBody*> getBeybladeBodies() const { return beybladeBodies; }
    std::vector<StadiumBody*> getStadiumBodies() const { return stadiumBodies; }

private:
    double fluidDrag;
    std::vector<BeybladeBody*> beybladeBodies;
    std::vector<StadiumBody*> stadiumBodies;
    const double SPIN_THRESHOLD = 30;
    //static void resolveCollision(RigidBody* bodyA, RigidBody* bodyB);
};
