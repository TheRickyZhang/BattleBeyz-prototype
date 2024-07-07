#include "Camera.h"

Camera::Camera(const glm::vec3& position, float yaw, float pitch, float roll) :
        Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f), Zoom(45.0f),
        Position(position), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)), Yaw(yaw), Pitch(pitch), Roll(roll),
        worldBoundingBox(glm::vec3(-100.0f), glm::vec3(100.0f)), boundingBox() {
    updateCameraVectors();
    updateBoundingBox();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::applyBoundaries(glm::vec3& position) const {
    // Add things here
}

void Camera::processKeyboard(int direction, float deltaTime, bool boundCamera) {
    float velocity = MovementSpeed * deltaTime;
    glm::vec3 newPosition = Position;

    if (direction == GLFW_KEY_W)
        newPosition += Front * velocity;
    if (direction == GLFW_KEY_S)
        newPosition -= Front * velocity;
    if (direction == GLFW_KEY_A)
        newPosition -= Right * velocity;
    if (direction == GLFW_KEY_D)
        newPosition += Right * velocity;
    if (direction == GLFW_KEY_Q)
        newPosition -= Up * velocity;
    if (direction == GLFW_KEY_E)
        newPosition += Up * velocity;
    if (direction == GLFW_KEY_ESCAPE)
        Zoom = 1.0f;

    if (boundCamera) {
        applyBoundaries(newPosition);
    }

    Position = newPosition;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    MovementSpeed += yoffset * 0.5f;
    if (MovementSpeed < 0.1f)
        MovementSpeed = 0.1f;
    if (MovementSpeed > 10.0f)
        MovementSpeed = 10.0f;
}

void Camera::updateCameraVectors() {
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0
    Up = glm::normalize(glm::cross(Right, Front));       // the more you look up or down which results in slower movement.

    // Apply roll
    glm::mat4 rot = glm::rotate(glm::mat4(1.0f), glm::radians(Roll), Front);
    Right = glm::vec3(rot * glm::vec4(Right, 0.0f));
    Up = glm::vec3(rot * glm::vec4(Up, 0.0f));
}

void Camera::updateBoundingBox() {
    boundingBox.min = Position - glm::vec3(0.01f);
    boundingBox.max = Position + glm::vec3(0.01f);
}
