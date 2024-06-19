//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "TextRenderer.h"
#include "ShaderPath.h"

#include <iostream>
#include <sstream>
#include "Buffers.h"
#include "Stadium.h"
#include <iomanip>

// GLOBAL VARIABLES

// Initial camera values
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Window dimensions
int windowWidth, windowHeight;
int minWidth, minHeight;
const float aspectRatio = 16.0f / 9.0f;

// Last known mouse positions (initialized to center of the screen)
double lastX = 400, lastY = 300;
bool firstMouse = true;

// Euler angles for rotation
double yaw = -90.0f;  // Yaw is initialized to -90.0 degrees to look along the negative Z-axis
double pitch = 0.0f;
double roll = 0.0f;

// Sensitivity
const float sensitivity = 0.1f;
float cameraSpeed = 0.05f;  // Speed of the camera movement per frame

glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;

// Declare globally for callback functions to use
ShaderProgram* objectShader;

// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (width == windowWidth && height == windowHeight) {
        return;
    }
    windowWidth = width;
    windowHeight = height;
    int newWidth = width;
    int newHeight = static_cast<int>(width / aspectRatio);
    if (newHeight > height) {
        newHeight = height;
        newWidth = static_cast<int>(height * aspectRatio);
    }
    // Adjust window size to maintain the aspect ratio
    if (newWidth != width || newHeight != height) {
        glfwSetWindowSize(window, newWidth, newHeight);
    }

    glViewport(0, 0, newWidth, newHeight);
    projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    objectShader->use();
    objectShader->setUniformMat4("projection", projection);
}


// Change camera speed or zoom in/out
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // change camera speed
    cameraSpeed += yoffset * 0.01f;
    if (cameraSpeed < 0.01f)
        cameraSpeed = 0.01f;
    if (cameraSpeed > 1.0f)
        cameraSpeed = 1.0f;
}

// Function to handle mouse movement
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    // Right mouse button = rotate camera
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        double xoffset = xpos - lastX;
        double yoffset = lastY - ypos; // Y-reversed
        lastX = xpos;
        lastY = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;
        yaw += xoffset;
        pitch += yoffset;

        // Constrain the pitch angle to avoid flipping at the poles
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Update camera front vector based on Euler angles
        glm::vec3 front;
        front.x = float (cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
        front.y = float (sin(glm::radians(pitch)));
        front.z = float (sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        cameraFront = glm::normalize(front);
    } else {
        // Avoid sudden jumps when right button is pressed again
        firstMouse = true;
    }
}

// Function to process input from the keyboard
void processInput(GLFWwindow* window) {
    // Calculate right vector based on current front and up vectors
    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // WASD for lateral, QE for vertical movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraRight;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraUp;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window. Note you NEED to call these functions to take in input from mouse and keyboard
    GLFWwindow* window = glfwCreateWindow(800, 600, "BattleBeyz", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    minWidth = 800;
    minHeight = static_cast<int>(minWidth / aspectRatio);
    glfwSetWindowSizeLimits(window, minWidth, minHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Set color blinding and depth testing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    // Initialize ShaderProgram for 3D objects
    objectShader = new ShaderProgram("../assets/shaders/object.vs", "../assets/shaders/object.fs");

// Initialize font rendering
    TextRenderer textRenderer("../assets/fonts/paladins.ttf", 800, 600);

    GLuint tetrahedronVAO, tetrahedronVBO, tetrahedronEBO;
    float tetrahedronVertices[] = {
            // Positions         // Normals        // TexCoords // Colors
            0.0f,  1.0f,  0.0f,  0.0f,  0.5773f,  0.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // Top vertex (Red)
            0.0f,  0.0f, -1.0f,  0.0f,  0.5773f, -0.8165f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // Front vertex (Green)
            -1.0f,  0.0f,  1.0f, -0.8165f,  0.5773f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Left vertex (Blue)
            1.0f,  0.0f,  1.0f,  0.8165f,  0.5773f,  0.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f  // Right vertex (Yellow)
    };

    unsigned int tetrahedronIndices[] = {
            0, 1, 2, // Front face
            0, 1, 3, // Right face
            0, 2, 3, // Left face
            1, 2, 3  // Bottom face
    };
    setupBuffers(tetrahedronVAO, tetrahedronVBO, tetrahedronEBO, tetrahedronVertices, sizeof(tetrahedronVertices), tetrahedronIndices, sizeof(tetrahedronIndices));

    // Initialize VAO, VBO, and EBO for the floor
    GLuint floorVAO, floorVBO, floorEBO;
    float floorVertices[] = {
            // Positions         // Normals        // TexCoords  // Colors
            -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,
            10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f,
            10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.5f,
            -10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.5f, 0.5f,
    };
    unsigned int floorIndices[] = {
            0, 1, 2,
            2, 3, 0
    };
    setupBuffers(floorVAO, floorVBO, floorEBO, floorVertices, sizeof(floorVertices), floorIndices, sizeof(floorIndices));

    // Initialize VAO, VBO, and EBO for the floor
    GLuint stadiumVAO = 0, stadiumVBO = 0, stadiumEBO = 0;
//    glGenVertexArrays(1, &stadiumVAO);
//    glGenBuffers(1, &stadiumVBO);
//    glGenBuffers(1, &stadiumEBO);
    // Create the Stadium object
    auto stadiumPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 stadiumColor = glm::vec3(0.8f, 0.8f, 0.8f);
    float stadiumRadius = 4.0f;
    float stadiumCurvature = 0.01f;
    int numRings = 3;
    int sectionsPerRing = 8;

    Stadium stadium(stadiumVAO, stadiumVBO, stadiumEBO, stadiumPosition, stadiumColor,
                    stadiumRadius, stadiumCurvature, numRings, sectionsPerRing);

    // Initial matrices for model, view, and projection
    // Identity matrix
    model = glm::mat4(1.0f);
    // Position camera at (0, 0, 3) and look at (0, 0, 0) with the up vector pointing in the positive y direction
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // Project 45 degree view with 4:3 aspect ratio
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Initialize default shader program with the model, view, and projection matrices. Also sets to use.
    objectShader->setUniforms(model, view, projection);

    // Main input loop
    while (!glfwWindowShouldClose(window)) {
        // Process input (keyboard, mouse, etc.)
        processInput(window);

        // Clear the color and depth buffers to prepare for a new frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        objectShader->use();
        // Update the view matrix based on the current camera position and orientation
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        objectShader->setUniformMat4("view", view);
        objectShader->setUniformMat4("projection", projection);
        objectShader->setUniformMat4("model", model);

        // Set light properties and view position
        objectShader->setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        objectShader->setUniformVec3("lightPos", glm::vec3(0.0f, 1e6f, 0.0f)); // Light position very high in the y-direction
        objectShader->setUniformVec3("viewPos", cameraPos);

        // Render the floor
        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

//        // Render the triangle
//        glBindVertexArray(tetrahedronVAO);
//        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

        // Update and render the stadium
        stadium.render(*objectShader);

        // Inside the main render loop, before rendering the text. TOFIX: doesn't render negative sign correctly
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "X: " << cameraPos.x << " "
           << "Y: " << cameraPos.y << " "
           << "Z: " << cameraPos.z;
        std::string cameraPosStr = ss.str();

        // Render the camera position
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        textRenderer.Resize(windowWidth, windowHeight);
        textRenderer.RenderText(cameraPosStr, 25.0f, windowHeight - 50.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up resources
    glDeleteVertexArrays(1, &tetrahedronVAO);
    glDeleteBuffers(1, &tetrahedronVBO);
    glDeleteBuffers(1, &tetrahedronEBO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &floorEBO);

    // Terminate GLFW to clean up allocated resources
    glfwTerminate();
    return 0;
}