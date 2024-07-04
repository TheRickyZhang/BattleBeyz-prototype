#pragma once

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Camera.h"
#include "QuadRenderer.h"
#include "imgui.h"


// All data needed to be passed to the callback functions
struct CallbackData {
    int *windowWidth;
    int *windowHeight;
    float aspectRatio;
    glm::mat4 *projection;
    ShaderProgram *shader;
    ShaderProgram* backgroundShader;
    CameraState *cameraState;
    QuadRenderer *quadRenderer;
    bool showHomeScreen;
    bool showInfoScreen;
    bool showCustomizeScreen;
    bool showAboutScreen;
    ImFont* defaultFont;
    ImFont* titleFont;
    ImFont* attackFont;


    CallbackData(int *width, int *height, float ratio, glm::mat4 *proj, ShaderProgram *sh, ShaderProgram* background,
                 CameraState *camState, QuadRenderer *quadRend, bool showHome, bool showInfo,
                 bool showCustomize, bool showAbout, ImFont* defaultF, ImFont* titleF, ImFont* attackF)
            : windowWidth(width), windowHeight(height), aspectRatio(ratio), projection(proj), shader(sh),
            backgroundShader(background), cameraState(camState), quadRenderer(quadRend), showHomeScreen(showHome),
            showInfoScreen(showInfo), showCustomizeScreen(showCustomize), showAboutScreen(showAbout),
            defaultFont(defaultF), titleFont(titleF), attackFont(attackF) {}
};