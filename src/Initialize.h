#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Initialize GLFW and GLEW
GLFWwindow* initGLFWandGLEW(const char* title, int width, int height,
                            bool fullscreen = false, bool resizable = true,
                            int glMajor = 3, int glMinor = 3,
                            int samples = 4, int refreshRate = GLFW_DONT_CARE);
