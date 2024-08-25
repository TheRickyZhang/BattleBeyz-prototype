////////////////////////////////////////////////////////////////////////////////
// Physics.cpp -- Physics code -- rz -- 2024-08-08
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#include "Physics.h"

/**
* Calculate air resistance proportional to C * v^2 for both angular and linear components
* TODO: I think linear and angular values should be calculated differently
*/

void Physics::accumulateAirResistance(BeybladeBody* beyblade, double fluidDrag) {
    //double surfaceArea = M_PI * beyblade->getLayerRadius() * beyblade->getLayerRadius() * beyblade->getLayerHeight();
    double totalDragCoefficient = 0.5 * fluidDrag;
    glm::vec3 angularAirResistance = -dv3(totalDragCoefficient) * beyblade->getAngularVelocity() * glm::length(beyblade->getAngularVelocity());
    glm::vec3 linearAirResistance = -dv3(totalDragCoefficient) * beyblade->getVelocity() * glm::length(beyblade->getVelocity());
    beyblade->accumulateAngularAccelaration(angularAirResistance);
    beyblade->accumulateAccelaration(linearAirResistance);
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

    // Linear friction caused by contact
    glm::vec3 frictionAccelaration = dv3(combinedCOF) * glm::cross(beyblade->getAngularVelocity(), stadiumNormal);
    double accelMagnitude = glm::length(frictionAccelaration);

    // Prevent case where Beyblade and Stadium are perfectly aligned, and nothing moves
    if (accelMagnitude < 0.005) {
        frictionAccelaration = glm::vec3(0.005, 0, 0);
    }

    glm::vec3 angularFrictionAccelaration = -beyblade->getAngularVelocity() * glm::length(frictionAccelaration) / dv3(accelMagnitude);
    glm::vec3 linearFrictionAccelaration = dv3(Physics::FRICTIONAL_EFFICIENCY) * frictionAccelaration;

    beyblade->accumulateAngularAccelaration(angularFrictionAccelaration);
    beyblade->accumulateAccelaration(linearFrictionAccelaration);

    /*
    * Previous more complicated code (Maybe consult with a physics major. Not urgent)
    * 
    // Calculate direction using displacement
    //glm::vec3 unitDisplacement = stadium.getCenter() - beyBottomPosition;
    //unitDisplacement.y = 0.0f;
    // Use stadium normal for friction calculations
    glm::vec3 stadiumNormal = stadium.getNormal(beyblade.getCenter().x, beyblade.getCenter().z);

    glm::vec3 beybladeNormal = beyblade.getNormal();

    double combinedCOF = (stadium.getCOF() + beyblade.getDriverCOF()) / 2;

    double cosineOfAngle = glm::dot(stadiumNormal, glm::vec3(0.0, 1.0, 0.0));
    double frictionalForceMagnitude = combinedCOF * beyblade.getMass() * Physics::GRAVITY * cosineOfAngle;

    glm::vec3 frictionalForce = glm::cross(beybladeNormal, glm::vec3(beyblade.getAngularVelocity()) * glm::vec3((float)frictionalForceMagnitude));

    // Might be the case that if normals align, no frictional force is applied
    if (frictionalForce != glm::vec3(0.0)) {
        return frictionalForce;
    }
    else {
        return glm::vec3(0.01, 0.00, 0.00);
    }
    */
}

/**
* Apply a linear force to the Beyblade based on F = mu * m * g * cos(theta) * unit(displacement)
*/
void Physics::accumulateSlope(BeybladeBody* beyblade, StadiumBody* stadium)
{
    glm::vec3 beybladeNormal = beyblade->getNormal();
    glm::vec3 stadiumNormal = stadium->getNormal(beyblade->getCenter().x, beyblade->getCenter().z);
    double cosineOfAngle = glm::dot(stadiumNormal, beybladeNormal);
    double combinedCOF = (stadium->getCOF() + beyblade->getDriverCOF()) / 2;

    glm::vec3 beyBottomPosition = beyblade->getBottomPosition();
    glm::vec3 unitDisplacement = stadium->getCenter() - beyBottomPosition;

    glm::vec3 slopeForce = dv3(Physics::GRAVITY * cosineOfAngle * combinedCOF) * unitDisplacement;

    beyblade->accumulateAccelaration(slopeForce);
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