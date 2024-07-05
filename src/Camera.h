#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera {
public:
    explicit Camera(const glm::vec3& position, float yaw = -90.0f, float pitch = 0.0f, float roll = 0.0f);

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up{};
    glm::vec3 Right{};
    glm::vec3 WorldUp;

    // Euler Angles (add roll)
    float Yaw;
    float Pitch;
    float Roll;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    glm::vec3 minBound;
    glm::vec3 maxBound;

    // Camera matrices
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    void processKeyboard(int direction, float deltaTime, bool boundCamera);
    void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void processMouseScroll(float yoffset);

private:
    void updateCameraVectors();
    void applyBoundaries(glm::vec3 &position) const;
};

struct CameraState {
    Camera* camera;
    double lastX;
    double lastY;
    bool firstMouse;

    CameraState(Camera* cam, double startX, double startY)
            : camera(cam), lastX(startX), lastY(startY), firstMouse(true) {}
};