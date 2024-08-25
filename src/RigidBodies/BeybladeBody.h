////////////////////////////////////////////////////////////////////////////////
// BeybladeBody.h -- Beyblade Physics include -- rz -- 2024-08-15
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "BeybladeParts.h"
#include "../BoundingBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <optional>
#include "../Utils.h"

/**
 * BeybladeBody. Contains all of the physical properties of a beyblade.
 * 
 * Can be initialized with default values or with Layer, Disc, and Driver objects.	See typical SI values in units.txt
 */
class BeybladeBody {
public:
	BeybladeBody::BeybladeBody(Layer layer, Disc disc, Driver driver);
	BeybladeBody::BeybladeBody();

	// Simple getters
	glm::vec3 BeybladeBody::getCenter() const { return baseCenter; }
	glm::vec3 BeybladeBody::getVelocity() const { return velocity; }
	glm::vec3 BeybladeBody::getAngularVelocity() const { return angularVelocity; }
	double BeybladeBody::getLayerHeight() const { return layerHeight; }
	double BeybladeBody::getMass() const { return mass; }
	double BeybladeBody::getDriverCOF() const { return coefficientOfFriction; }
	double BeybladeBody::getLayerCOR() const { return coefficientOfRestitution; }
	double BeybladeBody::getLayerRadius() const { return layerRadius; }

	// Specialized getters
	double BeybladeBody::getAngularVelocityMagnitude() const { return glm::length(angularVelocity); }
	bool isSpinningClockwise() const { return angularVelocity.y < 0; }
	glm::vec3 BeybladeBody::getNormal() const;
	glm::vec3 BeybladeBody::getBottomPosition() const;

	// Setters
	void BeybladeBody::setInitialLaunch(glm::vec3 initialCenter, glm::vec3 initialVelocity, glm::vec3 initialAngularVelocity);

	// Adjustors
	void BeybladeBody::addCenterY(double addY) { baseCenter.y += static_cast<float>(addY); }
	void BeybladeBody::addCenterXZ(double addX, double addZ) { baseCenter.x += static_cast<float>(addX); baseCenter.z += static_cast<float>(addZ); }

	// Used in collision calculations
	double sampleRecoil();
	static std::optional<double> distanceOverlap(BeybladeBody* a, BeybladeBody* b);

	// Accumulators
	void accumulateVelocity(glm::vec3 addedVelocity);
	void accumulateAngularVelocity(glm::vec3 addedAngularVelocity);
	void accumulateAccelaration(glm::vec3 addedAccelaration);
	void accumulateAngularAccelaration(glm::vec3 addedAngularAccelaration);

	void accumulateImpulseMagnitude(double magnitude);
	void accumulateAngularImpulseMagnitude(double magnitude);

	// Updators: these are the ones that significantly change the values of the body!
	void applyAccumulatedChanges(double deltaTime);
	void update(double deltaTime);

	std::vector<BoundingBox*> boundingBoxes{};
private:
	// Global Position
	glm::vec3 baseCenter{};

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

	// Accumulated delta velocity to be applied at cycle end (for instantaneous collisions, does not depend on deltaTime)
	glm::vec3 accumulatedVelocity {};
	glm::vec3 accumulatedAngularVelocity {};

	// Accumulated delta accelaration to be applied at cycle end (for forces like friction and inclined plane that do depend on time)
	glm::vec3 accumulatedAccelaration {};
	glm::vec3 accumulatedAngularAccelaration{};
};