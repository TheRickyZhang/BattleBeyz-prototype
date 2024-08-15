////////////////////////////////////////////////////////////////////////////////
// Utils.cpp -- Utility routines -- rz -- 2024-08-08
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#include "Utils.h"

/**
* Convert screen to world coordinates.  TODO: Params.
*/

glm::vec3 screenToWorldCoordinates(GLFWwindow* window, double xpos, double ypos, const glm::mat4& view, const glm::mat4& projection) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float x = (2.0f * (float)xpos) / width - 1.0f;
    float y = 1.0f - (2.0f * (float)ypos) / height;
    float z = 1.0f;

    glm::vec3 ray_nds = glm::vec3(x, y, z);
    glm::vec4 ray_clip = glm::vec4(ray_nds, 1.0f);
    glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0.0f);

    glm::vec3 ray_wor = glm::vec3(glm::inverse(view) * ray_eye);
    ray_wor = glm::normalize(ray_wor);

    return ray_wor;
}

/**
* Perform intersection test with objects in your scene and return the object name.
* 
* TODO: Replace this with your actual intersection logic
*/

std::string checkIntersection(const glm::vec3& ray_world) {
    if (glm::length(ray_world - glm::vec3(0.0f, 0.0f, 0.0f)) < 1.0f) {
        return "Object1";
    }
    return "None";
}

/**
* Print an error message if an OpenGL error occured.
*/

void checkGLError(const char* stmt, const char* fname, int line) {
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL error " << err << " at " << fname << ":" << line << " for " << stmt << std::endl;
    }
}

/**
* OpenGL cleanup.
*/

void cleanup(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}