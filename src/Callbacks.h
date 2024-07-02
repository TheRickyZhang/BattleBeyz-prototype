#pragma once
#include "Camera.h"
#include "CallbackData.h"
#include <GLFW/glfw3.h>

// Define callbacks to be used in main
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window,float deltaTime);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

