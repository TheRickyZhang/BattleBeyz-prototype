////////////////////////////////////////////////////////////////////////////////
// BeybladeBody.h -- Beyblade Physics include -- rz -- 2024-08-15
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BeybladeParts.h"
#include "../BoundingBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

/**
 * BeybladeBody. Contains all of the physical properties of a beyblade.
 * 
 * Can be initialized with default values or with Layer, Disc, and Driver objects.
 * Ordinary value ranges (SI Units):
 * - layer radius: 0.02 to 0.03
 * - layer height: 0.008 to 0.012
 * - driver radius: 0.0005 to 0.0025
 * - driver height: 0.01 to 0.02
 * - mass: 0.03 to 0.1
 * - moment of inertia: 8*10^-6 to 2*10^-5
 * 
 * Remember: clockwise = negative y, counterclockwise = positive y for angular velocity
 */
class BeybladeBody {
public:
	BeybladeBody::BeybladeBody(Layer layer, Disc disc, Driver driver) :
		layerRadius(layer.radius), layerHeight(layer.height), discRadius(disc.radius), discHeight(disc.height),
		driverRadius(driver.radius), driverHeight(driver.height), recoilDistribution(layer.recoilDistribution),
		coefficientOfRestitution(0.8), coefficientOfFriction(0.2),
		dragCoefficient(0.7 * layer.dragCoefficient + 0.2 * disc.dragCoefficient + 0.1 * driver.dragCoefficient),
		mass(layer.mass + disc.mass + driver.mass), velocity(glm::vec3(0.0)), acceleration(glm::vec3(0.0)),
		momentOfInertia(layer.momentOfInertia + disc.momentOfInertia + driver.momentOfInertia) {}

	glm::vec3 BeybladeBody::getCenter() const { return baseCenter; }
	std::vector<BoundingBox*> boundingBoxes{};
private:
	// Global Position
	glm::vec3 baseCenter{};

	// TODO: Determine whether this is necessary given angularVelocity already encodes direction
	// glm::vec3 axisOfRotation{ 0.0, 1.0, 0.0 };

	// Measurements
	double layerRadius;
	double layerHeight;
	double discRadius;
	double discHeight;
	double driverRadius;
	double driverHeight;

	// Contact Properties
	RandomDistribution* recoilDistribution;
	double coefficientOfRestitution;
	double coefficientOfFriction;
	double dragCoefficient;

	// Linear Physics
	double mass;
	glm::vec3 velocity{};
	glm::vec3 acceleration{};

	// Rotational Physics
	double momentOfInertia;
	glm::vec3 angularVelocity{ 0.0, 1.0, 0.0 };
	glm::vec3 angularAcceleration{};
};