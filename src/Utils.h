////////////////////////////////////////////////////////////////////////////////
// Utils.h -- Utility routines include -- rz -- 2024-08-08
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stb_image.h>

// Macro to wrap OpenGL calls for error checking
#define GL_CHECK(stmt) do { \
        stmt; \
        checkGLError(#stmt, __FILE__, __LINE__); \
    } while (0)


glm::vec3 screenToWorldCoordinates(GLFWwindow* window, double xpos, double ypos, const glm::mat4& view, const glm::mat4& projection);
std::string checkIntersection(const glm::vec3& ray_world);
void checkGLError(const char* stmt, const char* fname, int line);
void cleanup(GLFWwindow* window);

enum ProgramState {
    LOADING,
    RENDERING,
    ACTIVE
};

