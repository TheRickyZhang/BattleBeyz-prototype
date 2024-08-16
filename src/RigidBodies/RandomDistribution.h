/////////////////////////////////////////////////////////////////////////////
// RandomDistribution.h -- Beyblade include -- rz -- 2024-08-15
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RandomDistribution.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <random>

/**
 * Represents a lognormal ditribution for use in calculating recoil strength. Scales both linear and angular impulse.
 * @param mean, stddev: the mean and standard deviation of the distribution. Scaled so still applies to lognormal distribution.
 */
class RandomDistribution {
public:
    RandomDistribution(double mean, double stddev)
        : rng(std::random_device{}())
    {
        double mu = std::log(mean * mean / std::sqrt(stddev * stddev + mean * mean));
        double sigma = std::sqrt(std::log(1 + (stddev * stddev) / (mean * mean)));
        distribution = std::lognormal_distribution<double>(mu, sigma);
    }

    /**
    * Samples the distribution and scales it by the speed the beys collide into each other.
    * @param angularSpeed: the absolute difference in angular velocity between the two beys
    * Typically 25 to 1000 rad/s (4 - 150 rev/s)
    * Angular factor increases from 0 to 500 and gently decreases from 500 to 1000, modeling empirical data (AKA I just made it up)
    *
    * @param collisionSpeed: the absolute difference in linear velocity between the two beys
    * Typically 0 to 0.5 m/s. Directly scales the distribution
    *
    * @return the nonegative scaled random number
    */
    double sample(double angularSpeed, double collisionSpeed) {
        double scalingFactor = (std::log(angularSpeed + 1) - (angularSpeed / 501)) * collisionSpeed;
        return std::min(scalingFactor, 4.0) * std::sqrt(collisionSpeed) * distribution(rng);
    }

private:
    std::mt19937 rng;
    std::lognormal_distribution<double> distribution;
};