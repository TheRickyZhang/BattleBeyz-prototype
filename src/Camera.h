#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(const glm::vec3& position, float yaw = -90.0f, float pitch = 0.0f);

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up{};
    glm::vec3 Right{};
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Camera matrices
    glm::mat4 getViewMatrix() const;
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yoffset);

private:
    void updateCameraVectors();
};

struct CameraState {
    Camera* camera;
    double lastX;
    double lastY;
    bool firstMouse;

    CameraState(Camera* cam, double startX, double startY)
            : camera(cam), lastX(startX), lastY(startY), firstMouse(true) {}
};