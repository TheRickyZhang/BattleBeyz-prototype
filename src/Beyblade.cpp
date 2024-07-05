//#include "Beyblade.h"
//#include <glm/gtc/matrix_transform.hpp>
//
//Beyblade::Beyblade()
//        : center(0.0f), rotationAxis(0.0f, 1.0f, 0.0f), precessionAxis(1.0f, 0.0f, 0.0f),
//          velocity(0.0f), angularVelocity(0.0f), tiltAngle(0.0f) {
//    // Initialization code
//}
//
//Beyblade::~Beyblade() {
//    // Cleanup code
//}
//
//void Beyblade::init() {
//    // Initialize buffers and load texture
//    buffers.init();
//    texture.load("path/to/texture.png");
//}
//
//void Beyblade::update(float deltaTime) {
//    // Update position based on velocity
//    center += velocity * deltaTime;
//
//    // Update rotation axis based on precession and tilt
//    rotationAxis = glm::rotate(rotationAxis, tiltAngle, precessionAxis);
//
//    // Update other properties as needed
//}
//
//void Beyblade::render(const ShaderProgram& shader) {
//    shader.use();
//    texture.bind();
//    buffers.bind();
//
//    // Compute model matrix based on position and rotation
//    glm::mat4 model = glm::translate(glm::mat4(1.0f), center);
//    model = glm::rotate(model, angularVelocity, rotationAxis);
//
//    // Set uniforms
//    shader.setUniform("model", model);
//
//    // Draw call
//    glDrawElements(GL_TRIANGLES, buffers.getIndexCount(), GL_UNSIGNED_INT, 0);
//
//    buffers.unbind();
//    texture.unbind();
//}
