#include <glm/glm.hpp>
#include "ShaderProgram.h"

// All data needed to be passed to the callback functions
struct CallbackData {
    int* windowWidth;
    int* windowHeight;
    float aspectRatio;
    glm::mat4* projection;
    ShaderProgram* shader;
    CameraState* cameraState;

    CallbackData(int* width, int* height, float ratio, glm::mat4* proj, ShaderProgram* sh, CameraState* camState)
            : windowWidth(width), windowHeight(height), aspectRatio(ratio), projection(proj), shader(sh), cameraState(camState) {}
};