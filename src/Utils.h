#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

std::string readFile(const char* filePath);
glm::vec3 screenToWorldCoordinates(GLFWwindow* window, double xpos, double ypos, const glm::mat4& view, const glm::mat4& projection);
std::string checkIntersection(const glm::vec3& ray_world);