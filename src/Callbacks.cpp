#include "Callbacks.h"

// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if (!data || width == *(data->windowWidth) && height == *(data->windowHeight)) {
        return;
    }

    *(data->windowWidth) = width;
    *(data->windowHeight) = height;

    int newWidth = width;
    int newHeight = static_cast<int>(width / data->aspectRatio);
    if (newHeight > height) {
        newHeight = height;
        newWidth = static_cast<int>(height * data->aspectRatio);
    }
    if (newWidth != width || newHeight != height) {
        glfwSetWindowSize(window, newWidth, newHeight);
    }

    glViewport(0, 0, newWidth, newHeight);
    *(data->projection) = glm::perspective(glm::radians(45.0f), (float)newWidth / newHeight, 0.1f, 100.0f);
    data->shader->use();
    data->shader->setUniformMat4("projection", *(data->projection));
}

// Move camera on right click
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if (!data || !data->cameraState) return;
    CameraState* cameraState = data->cameraState;

    if (cameraState->firstMouse) {
        cameraState->lastX = xpos;
        cameraState->lastY = ypos;
        cameraState->firstMouse = false;
    }

    double xoffset = xpos - cameraState->lastX;
    double yoffset = cameraState->lastY - ypos;
    cameraState->lastX = xpos;
    cameraState->lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        cameraState->camera->processMouseMovement(xoffset, yoffset);
    } else {
        cameraState->firstMouse = true;
    }
}

// Handle key presses
void processInput(GLFWwindow* window, float deltaTime) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if (!data || !data->cameraState) return;
    CameraState* cameraState = data->cameraState;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_W, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_S, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_A, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_D, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_Q, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_E, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        cameraState->camera->processKeyboard(GLFW_KEY_ESCAPE, deltaTime);
}

// Change camera speed or zoom in/out
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    if (!data || !data->cameraState) return;
    CameraState* cameraState = data->cameraState;

    cameraState->camera->processMouseScroll(yoffset);
}