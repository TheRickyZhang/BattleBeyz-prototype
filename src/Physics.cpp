////////////////////////////////////////////////////////////////////////////////
// Physics.cpp -- Physics code -- rz -- 2024-08-08
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#include "Physics.h"

/**
* Calculate air resistance proportional to C * v^2 for both angular and linear components
* TODO: I think linear and angular values should be calculated differently
*/

void Physics::accumulateAirResistance(BeybladeBody* beyblade, double airDensity) {
    // b = 1/2 * Cd * A * p
    // We assume that linear drag coefficient is always 0.9 for now, based on cylinder approximation
    double linearDragConstant = beyblade->getLinearDragTerm() * airDensity;
    double velocityMagnitude = glm::length(beyblade->getVelocity());

    // Adrag = (-b * v^2 / mass) * unit v
    glm::vec3 linearAirResistanceAcceleration = -dv3(linearDragConstant * velocityMagnitude / beyblade->getMass()) * beyblade->getVelocity();

    // b = 1/2 * Cd * A * r^2 * p
    double angularDragConstant = beyblade->getAngularDragTerm() * airDensity;
    double angularVelocityMagnitude = glm::length(beyblade->getAngularVelocity());

    // Adrag = (-b * w^2 / moi) * unit w
    glm::vec3 angularAirResistanceAcceleration = -dv3(angularDragConstant * angularVelocityMagnitude / beyblade->getMomentOfInertia()) * beyblade->getAngularVelocity();

    beyblade->accumulateAngularAcceleration(angularAirResistanceAcceleration);
    beyblade->accumulateAcceleration(linearAirResistanceAcceleration);
}


/**
* TODO: Check if force is applied in the right direction. Might be unintuitive if not familiar with Beyblade.
*
* Frictional force opposes rotationalVelocity, but increases linearVelocity tangential to movement.
*
* Calculate a coefficient of friction between the driver and the stadium
* Initial magnitude and direction of force is mu * (angularVelocity x stadiumNormal)
* 
* When applied to angular, change direction to oppose angularVelocity
* When applied to linear, change reduce magnitude by frictional efficiency
*/

void Physics::accumulateFriction(BeybladeBody* beyblade, StadiumBody* stadium) {
    // Gets the normal of the stadium at the beyblade's position
    glm::vec3 stadiumNormal = stadium->getNormal(beyblade->getCenter().x, beyblade->getCenter().z);

    // Gets the rotation axis of the Bey (this may be different than the stadium)
    glm::vec3 beybladeNormal = beyblade->getNormal();

    double combinedCOF = (stadium->getCOF() + beyblade->getDriverCOF()) / 2;
    glm::vec3 beyBottomPosition = beyblade->getBottomPosition();
    double stadiumY = stadium->getY(beyBottomPosition.x, beyBottomPosition.z);

    // Linear friction caused by contact. Always y=0 if beyblade is upright
    glm::vec3 normalizedAngularVelocity = glm::normalize(beyblade->getAngularVelocity());

    // This should be velocity, NOT accelaration!!!!
    glm::vec3 frictionDirectionAcceleration = glm::cross(normalizedAngularVelocity, stadiumNormal);
    glm::vec3 frictionAcceleration = dv3(combinedCOF * beyblade->getDriverRadius() * glm::length(beyblade->getAngularVelocity())) * frictionDirectionAcceleration;
    double accelMagnitude = glm::length(frictionAcceleration);

    // Prevent case where Beyblade and Stadium are perfectly aligned, and nothing moves
    if (accelMagnitude < 0.001) {
        frictionAcceleration = glm::vec3(0.001, 0, 0);
    }

    // These now seem to work fine together, BUT 
    // TOFIX: Beyblade runs out of spin too quickly!!! How should the friction accelaration be calculated?
    glm::vec3 angularFrictionAcceleration = -dv3(0.1) * glm::length(frictionAcceleration);
    glm::vec3 linearFrictionAcceleration = dv3(Physics::FRICTIONAL_EFFICIENCY) * frictionAcceleration;

    beyblade->accumulateAngularAcceleration(angularFrictionAcceleration);
    beyblade->accumulateAcceleration(linearFrictionAcceleration);
}

/**
* Apply a linear force to the Beyblade based on F = mu * m * g * cos(theta) * unit(displacement)
*/
void Physics::accumulateSlope(BeybladeBody* beyblade, StadiumBody* stadium)
{
    glm::vec3 beyBottomPosition = beyblade->getBottomPosition();
    glm::vec3 beybladeNormal = beyblade->getNormal();
    glm::vec3 stadiumNormal = stadium->getNormal(beyblade->getBottomPosition().x, beyblade->getBottomPosition().z);
    double combinedCOF = (stadium->getCOF() + beyblade->getDriverCOF()) / 2;

    if (glm::dot(stadiumNormal, beybladeNormal) <= 0.001f) {
        return;
    }
    glm::vec3 crossProduct = glm::cross(stadiumNormal, beybladeNormal);
    double sinOfAngle = glm::length(crossProduct) / (glm::length(stadiumNormal) * glm::length(beybladeNormal));

    // Check magnitudes here
    glm::vec3 unitDisplacement = glm::normalize(stadium->getCenter() - beyBottomPosition);
    glm::vec3 slopeForce = dv3(Physics::GRAVITY * sinOfAngle * combinedCOF) * unitDisplacement;
    std::cout << Physics::GRAVITY << " " << sinOfAngle << " " << combinedCOF << std::endl;
    std::cout << unitDisplacement.x << " " << unitDisplacement.y << " " << unitDisplacement.z << std::endl;
    printVec3("Slope force", slopeForce);
    beyblade->accumulateAcceleration(slopeForce);
}

/**
* Calculates changes in velocity due to both linear and angular contact
* 
* TODO: Perhaps this can be modularized more more? Especially to test each component individually
*/
void Physics::accumulateImpact(BeybladeBody* beyblade1, BeybladeBody* beyblade2, double contactDistance)
{
    // Goes from bey1 to bey2
    glm::vec3 center1Tocenter2 = beyblade2->getCenter() - beyblade1->getCenter();
    glm::vec3 unitSeparation = glm::normalize(center1Tocenter2);

    // Resolve clipping
    glm::vec3 displacement = dv3(0.5 * contactDistance) * unitSeparation;
    beyblade1->addCenterXZ(-displacement.x, -displacement.z);
    beyblade2->addCenterXZ(displacement.x, displacement.z);

    glm::vec3 velocity1 = beyblade1->getVelocity();
    glm::vec3 velocity2 = beyblade2->getVelocity();
    glm::vec3 vDiff = velocity2 - velocity1;
    double averageCOR = (beyblade1->getLayerCOR() + beyblade2->getLayerCOR()) / 2;

    double mass1 = beyblade1->getMass();
    double mass2 = beyblade2->getMass();

    // Trust this known linear collison model works correctly
    double impulseMagnitude = (1.0 + averageCOR) * glm::dot(vDiff, unitSeparation) /
        (1.0 / mass1 + 1.0 / mass2);
    glm::vec3 impulse = dv3(impulseMagnitude) * unitSeparation;

    glm::vec3 deltaVelocity1 = dv3(-impulseMagnitude / mass1) * unitSeparation;
    glm::vec3 deltaVelocity2 = dv3(impulseMagnitude / mass2) * unitSeparation;

    beyblade1->accumulateVelocity(deltaVelocity1);
    beyblade2->accumulateVelocity(deltaVelocity2);


    // Random effect with inherent attack power of beyblades built in
    double randomMagnitude = (beyblade1->sampleRecoil() + beyblade2->sampleRecoil());

    // Gets relative velocity moving towards each other.
    double linearCollisionSpeed = glm::dot(unitSeparation, velocity1) + glm::dot(-unitSeparation, velocity2);
    if (linearCollisionSpeed < 0) std::cerr << "Linear collision speed is less than 0!" << std::endl;

    // Different cases for same-spin vs opposite-spin collisions
    bool sameSpinDirection = beyblade1->isSpinningClockwise() == beyblade2->isSpinningClockwise();
    if (sameSpinDirection) {
        double angularSpeedDiff = beyblade1->getAngularVelocityMagnitude() + beyblade2->getAngularVelocityMagnitude();

        // Predictive modeling using y = log(x) - x/500 based on expected values for x.
        // Increases from AngularSpeedDiff = 0 to 500 and gently decreases from 500 to 1000, modeling empirical data (AKA I just made it up)
        double scalingFactor = (std::log(angularSpeedDiff + 1) - (angularSpeedDiff / 501)) * linearCollisionSpeed;

        // CHECK whether this conversion from angular to linear is correct.
        // Just simply multiply by COR since moment of inertia vs mass already accounts for?

        double recoilAngularImpulseMagnitude = randomMagnitude * std::max(scalingFactor, 4.0);

        beyblade1->accumulateAngularImpulseMagnitude(-recoilAngularImpulseMagnitude);
        beyblade2->accumulateAngularImpulseMagnitude(-recoilAngularImpulseMagnitude);

        double recoilLinearImpulseMagnitude = recoilAngularImpulseMagnitude * averageCOR;

        beyblade1->accumulateImpulseMagnitude(-recoilLinearImpulseMagnitude);
        beyblade2->accumulateImpulseMagnitude(-recoilLinearImpulseMagnitude);
    }
    else {
        // TODO
        double angularSpeedDiff = beyblade1->getAngularVelocityMagnitude() + beyblade2->getAngularVelocityMagnitude();
        std::cerr << "Opposite spin collisions have not been implemented yet";
    }
}

void Physics::preventStadiumClipping(BeybladeBody* beybladeBody, double deltaTime, StadiumBody* stadiumBody)
{
    glm::vec3 beyBottom = beybladeBody->getBottomPosition();
    double stadiumY = stadiumBody->getY(beyBottom.x, beyBottom.z);

    // Beyblade is clipping into stadium. Push it out along y-axis.
    if (stadiumY > beyBottom.y) {
        //std::cout << "Adjust bey pos from: " << beyBottom.y;
        beybladeBody->addCenterY(stadiumY - beyBottom.y);
        //beyBottom = beybladeBody->getBottomPosition();
        //std::cout << " to " << beyBottom.y << std::endl;
    }
}
