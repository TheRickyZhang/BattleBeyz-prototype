////////////////////////////////////////////////////////////////////////////////
// Physics.h -- Physics include -- rz -- 2024-08-08
// Copyright (c) 2024, Ricky Zhang.
// 
// CURRENTLY DOES NOTHING EXCEPT INCLUDE OTHER FILES
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#include "Utils.h"
#include "RigidBodies/BeybladeBody.h"
#include "RigidBodies/StadiumBody.h"

namespace Physics {
    constexpr double GRAVITY = 9.81;
    constexpr glm::vec3 GRAVITY_VECTOR = glm::vec3(0.0, -GRAVITY, 0.0);

    // How much loss in friction is converted to movement
    constexpr double FRICTIONAL_EFFICIENCY = 0.6;

    // How much aerial drag there is
    const double FLUID_DRAG = 0.08;
    void accumulateAirResistance(BeybladeBody* beyblade, double fluidDrag = FLUID_DRAG);

    void accumulateFriction(BeybladeBody* beyblade, StadiumBody* stadium);
    void accumulateSlope(BeybladeBody* beyblade, StadiumBody* stadium);
    void accumulateImpact(BeybladeBody* beyblade1, BeybladeBody* beyblade2, double contactDistance);
}


