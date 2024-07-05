//#pragma once
//
//#include "ShaderProgram.h"
//#include "Buffers.h"
//#include "Texture.h"
//#include <glm/glm.hpp>
//
//#include "GameObject.h"
//
//class Beyblade : GameObject {
//public:
//    Beyblade(unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3& pos, const glm::vec3& col,
//            const glm::vec3& ringColor, const glm::vec3& crossColor, float radius, float curvature, int numRings,
//            int verticesPerRing, float textureScale = 1.0f);
//    ~Beyblade();
//
//    void update() override {}
//    void initializeMesh() override;
//    void render(ShaderProgram &shader, const glm::vec3 &viewPos, const glm::vec3 &lightColor, const glm::vec3 &lightPos) override;
//
//protected:
//    void generateMeshData() override;
//
//private:
//    Texture texture;
//
//    glm::vec3 center;                // Center point
//    glm::vec3 rotationAxis;          // Axis of rotation
//    glm::vec3 precessionAxis;        // Axis of precession
//    glm::vec3 velocity;              // Translational velocity
//    float angularVelocity;           // Speed of rotation
//    float tiltAngle;                 // Tilt angle
//
//    // Placeholder for physics properties
//    // float mass;
//    // glm::vec3 position;
//};