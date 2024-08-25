////////////////////////////////////////////////////////////////////////////////
// PhysicsWorld.cpp -- Core physics handling -- rz -- 2024-08-08
// Copyright (c) 2024 Ricky Zhang
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "PhysicsWorld.h"

/**
* Add a beyblade body to the scene.
* 
* @param body               [in] A BeybladeBody object
*/

void PhysicsWorld::addBeybladeBody(BeybladeBody* body) {
    beybladeBodies.push_back(body);
}

/**
* Add a stadium body to the scene.
*
* @param body               [in] A StadiumBody object
*/

void PhysicsWorld::addStadiumBody(StadiumBody* body) {
    stadiumBodies.push_back(body);
}

void PhysicsWorld::removeBeybladeBody(BeybladeBody* body) {
    beybladeBodies.erase(std::remove(beybladeBodies.begin(), beybladeBodies.end(), body), beybladeBodies.end());
}

void PhysicsWorld::removeStadiumBody(StadiumBody* body) {
    stadiumBodies.erase(std::remove(stadiumBodies.begin(), stadiumBodies.end(), body), stadiumBodies.end());
}

/**
* Update body states based on time change. Collisions are detected first, then forces.
*
* @param deltaTime              [in] Time increment in seconds.
*/

void PhysicsWorld::update(float deltaTime) {
    /**
    * Resolve bey-stadium collisions
    */
    for (BeybladeBody* beybladeBody : beybladeBodies) {
        std::cout << "Beyblade center:" << beybladeBody->getCenter().x << ", " << beybladeBody->getCenter().y << ", " << beybladeBody->getCenter().z << std::endl;

        // TODO: If bey is below minimum spin threshold, end the match. Some default animation could be used.
        if (glm::length(beybladeBody->getAngularVelocity()) < SPIN_THRESHOLD) {
            // std::cerr << "Beyblade ran out of spin" << std::endl;
            return;
        }

        // Get position of the bottom tip
        glm::vec3 beyBottom = beybladeBody->getBottomPosition();

        // TODO: Refactor beybladeAcclearations to bey stored in Beyblade like accumulatedVelocity!
        // Should usually only be one stadium, but may need to scale to more
        for (StadiumBody* stadiumBody : stadiumBodies) {
            std::cout << "Stadium Center: " << stadiumBody->getCenter().x << ", " << stadiumBody->getCenter().y << ", " << stadiumBody->getCenter().z << std::endl;

            if(!stadiumBody->isInside(beyBottom.x, beyBottom.z)) {
                // Game is over since beyblade is out of bounds, implement behavior in future
                std::cerr << "Beyblade out of bounds" << std::endl;
                return;
            }

            //// Add air resistance
            //Physics::accumulateAirResistance(beybladeBody, fluidDrag);

            double stadiumY = stadiumBody->getY(beyBottom.x, beyBottom.z);

            // If the Beyblade is airborne by some significant amount, only apply gravity
            if (beyBottom.y - stadiumY > 0.005) {
                beybladeBody->accumulateAccelaration(Physics::GRAVITY_VECTOR);
            }

            // Beyblade is clipping into stadium. Push it out along y-axis.
            if (stadiumY > beyBottom.y) {
                beybladeBody->addCenterY(stadiumY - beyBottom.y);
                std::cout << "Added " << stadiumY - beyBottom.y << " to y" << std::endl;
                std::cout << "New y: " << beybladeBody->getCenter().y << std::endl;
            }

            //// Add friction and slope forces from contact
            //Physics::accumulateFriction(beybladeBody, stadiumBody);
            //Physics::accumulateSlope(beybladeBody, stadiumBody);
        }
    }
    /**
    * Resolve bey-bey collisions
    */
    for (int i = 0; i < beybladeBodies.size(); ++i) {
        for (int j = i + 1; j < beybladeBodies.size(); ++j) {  // TODO: Note that size() is size_t, not int. Best to cast loop counter or size() for clarity.
            BeybladeBody* bey1 = beybladeBodies[i];
            BeybladeBody* bey2 = beybladeBodies[j];
            std::optional<double> contactDistance = BeybladeBody::distanceOverlap(bey1, bey2);

            // Skip beys with no contact
            if (!contactDistance.has_value()) continue;

            /**
            * Linear repulsive force combines the collision due to initial velocity with the recoil from spins
            * Angular draining force is the loss of spin of both beys due to colliding
            */
            Physics::accumulateImpact(bey1, bey2, contactDistance.value());
        }
    }

    /**
    * Apply forces simultaneously by storing them, rather than sequentially which can cause issues (TODO: Such as what?).
    * Then, apply all at once to change velocities, then update positions with new velocities.
    */
    for (BeybladeBody* beybladeBody : beybladeBodies) {
        beybladeBody->applyAccumulatedChanges(deltaTime);
        beybladeBody->update(deltaTime);
    }
}

/**
* Resolve collisions. (Unused)
* 
* @param bodyA                  [in] First body.
* 
* @param bodyB                  [in] Second body.
*/

//void PhysicsWorld::resolveCollision(RigidBody* bodyA, RigidBody* bodyB) {
//    // Simple collision resolution logic
//    glm::vec3 relativeVelocity = bodyB->velocity - bodyA->velocity;
//    glm::vec3 collisionNormal = glm::normalize(bodyB->position - bodyA->position);
//    float velocityAlongNormal = glm::dot(relativeVelocity, collisionNormal);
//
//    if (velocityAlongNormal > 0) {
//        return;
//    }
//
//    float restitution = 0.5f; // Coefficient of restitution
//    float j = -(1 + restitution) * velocityAlongNormal;
//    j /= 1 / bodyA->mass + 1 / bodyB->mass;
//
//    glm::vec3 impulse = j * collisionNormal;
//    bodyA->velocity -= impulse / bodyA->mass;
//    bodyB->velocity += impulse / bodyB->mass;
//}

/**
* Debug render.
* 
* This currently shows all of the bounding boxes.  You can generate additional
* debug here.
* 
* @param shader                 [in] Our custom ShaderProgram.
*/

// TOFIX if not working?
void PhysicsWorld::renderDebug(ShaderProgram& shader) const {
    // Render all bounding boxes
    for (BeybladeBody* beybladeBody : beybladeBodies) {
        for (int i = 0; i < beybladeBody->boundingBoxes.size() /*&& i < 100*/; i++) {
            beybladeBody->boundingBoxes[i]->renderDebug(shader, beybladeBody->getCenter());
        }
    }
    for (StadiumBody* stadiumBody : stadiumBodies) {
        for (int i = 0; i < stadiumBody->boundingBoxes.size() /*&& i < 100*/; i++) {
            stadiumBody->boundingBoxes[i]->renderDebug(shader, stadiumBody->getCenter());
        }
    }
}
