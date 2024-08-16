#include "StadiumBody.h"

/**
* Checks whether a point is in the x-z plane of the stadium.
*/
bool StadiumBody::isInside(double x, double z) const {
    double scaledX = x - center.x;
    double scaledZ = z - center.z;
    return scaledX * scaledX + scaledZ * scaledZ <= radius * radius;
}

/**
* Returns the y-coordinate of the stadium at a given x and z.
*/
const double StadiumBody::getY(double x, double z) const {
    double scaledX = x - center.x;
    double scaledZ = z - center.z;
    double scaledY = scaledCurvature * (scaledX * scaledX + scaledZ * scaledZ);
    return scaledY + center.y;
}

/**
* Returns the unit normal of the stadium at a given x and z.
*/
const glm::vec3 StadiumBody::getNormal(double x, double z) const {
    double scaledX = x - center.x;
    double scaledZ = z - center.z;
    glm::vec3 normal = glm::normalize(glm::vec3(-2 * scaledCurvature * scaledX, 1.0, -2 * scaledCurvature * scaledZ));
    return normal;
}

/**
* TODO: Check if force is applied in the right direction.
* 
* Might be unintuitive if not familiar with Beyblade, but frictional force is applied tangentially to movement due to spin.
* 
* Calculate a coefficient of friction between the driver and the stadium
* Determine the frictional using weight * 
*/
const glm::vec3 StadiumBody::getFrictionForce(double x, double z, double mass, glm::vec3 angularVelocity, double driverCOF,
    double driverRadius) const {
    // Use stadium normal for friction calculations
    glm::vec3 stadiumNormal = getNormal(x, z);
    glm::vec3 beybladeNormal = getNormal(angularVelocity.x, angularVelocity.z);
	double combinedCOF = (coefficientOfFriction + driverCOF) / 2;

    double cosineOfAngle = glm::dot(stadiumNormal, glm::vec3(0.0, 1.0, 0.0));
    double frictionalForceMagnitude = combinedCOF * mass * Physics::GRAVITY * cosineOfAngle;

    glm::vec3 frictionalForce = glm::cross(beybladeNormal, angularVelocity) * glm::vec3(frictionalForceMagnitude);

    // Might be the case that if normals align, no frictional force is applied
    if(frictionalForce != glm::vec3(0.0)) {
        return frictionalForce;
    }
    else {
        return glm::vec3(0.01, 0.00, 0.00);
    }
}