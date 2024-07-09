#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "CallbackData.h"
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "Texture.h"
#include <windows.h>
#include "algorithm"

void showHomeScreen(GLFWwindow* window, Texture& homeScreenTexture, Texture& backgroundTexture);
void showInfoScreen(GLFWwindow* window, float (*imguiColor)[3]);
void showCustomizeScreen(GLFWwindow* window, Texture& backgroundTexture);
void showAboutScreen(GLFWwindow* window, Texture& backgroundTexture);
void showLoadingScreen(GLFWwindow* window, Texture& backgroundTexture, const char* message = "Loading...");