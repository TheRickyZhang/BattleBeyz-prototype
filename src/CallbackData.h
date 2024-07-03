#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Camera.h"


// All data needed to be passed to the callback functions
struct CallbackData {
    int* windowWidth;
    int* windowHeight;
    float aspectRatio;
    glm::mat4* projection;
    ShaderProgram* shader;
    CameraState* cameraState;
    bool showHomeScreen;
    bool showInfoScreen;
    bool showCustomizeScreen;
    bool showAboutScreen;

    CallbackData(int* width, int* height, float ratio, glm::mat4* proj, ShaderProgram* sh, CameraState* camState,
                 bool showHome = true, bool showInfo = false, bool showCustomize = false, bool showAbout = false)
            : windowWidth(width), windowHeight(height), aspectRatio(ratio), projection(proj), shader(sh), cameraState(camState),
            showHomeScreen(showHome), showInfoScreen(showInfo), showCustomizeScreen(showCustomize), showAboutScreen(showAbout) {}
};