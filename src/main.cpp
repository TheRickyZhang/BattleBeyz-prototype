#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "TextRenderer.h"
#include "ShaderSource.h"

#include <iostream>

// Callback function for when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Function to process input from the keyboard
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Basic Text Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Configure OpenGL state
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize TextRenderer
    TextRenderer textRenderer("../assets/fonts/MetalFight.ttf", 800, 600);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear the screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render text
        textRenderer.RenderText("Hello, world!", 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up and exit
    glfwTerminate();
    return 0;
}


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "ShaderProgram.h"
//#include "Buffers.h"
//#include "Initialize.h"
//#include "TextRenderer.h"
//#include "ShaderSource.h" // Include the header file
//
//#include <iostream>
//#include <filesystem>
//
//
//
//// GLOBAL VARIABLES
//
//
//// Callback function for when the window is resized
//void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}
//
//// Initial camera values
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//
//// Last mouse positions
//double lastX = 400, lastY = 300; // Assuming initial cursor position at the center of the screen
//bool firstMouse = true;
//
//// Euler angles for rotation
//double yaw = -90.0f;  // Yaw is initialized to -90.0 degrees to look along the negative Z-axis
//double pitch = 0.0f;
//double roll = 0.0f;
//
//// Sensitivity
//const float sensitivity = 0.1f;
//
//// Function to handle mouse movement
//void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
//    // Check if the right mouse button is held down
//    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
//        if (firstMouse) {
//            lastX = xpos;
//            lastY = ypos;
//            firstMouse = false;
//        }
//
//        double xoffset = xpos - lastX;
//        double yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
//        lastX = xpos;
//        lastY = ypos;
//
//        xoffset *= sensitivity;
//        yoffset *= sensitivity;
//
//        yaw += xoffset;
//        pitch += yoffset;
//
//        // Constrain the pitch angle to avoid flipping at the poles
//        if (pitch > 89.0f)
//            pitch = 89.0f;
//        if (pitch < -89.0f)
//            pitch = -89.0f;
//
//        // Update camera front vector based on Euler angles
//        glm::vec3 front;
//        front.x = float (cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
//        front.y = float (sin(glm::radians(pitch)));
//        front.z = float (sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
//        cameraFront = glm::normalize(front);
//    } else {
//        // Avoid sudden jumps when right button is pressed again
//        firstMouse = true;
//    }
//}
//
//// Function to process input from the keyboard
//void processInput(GLFWwindow* window) {
//    const float cameraSpeed = 0.05f;  // Speed of the camera movement per frame
//
//    // Calculate right vector based on current front and up vectors
//    glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, cameraUp));
//
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    // WASD for lateral, QE for vertical movement
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        cameraPos += cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraRight;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        cameraPos += cameraSpeed * cameraRight;
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
//        cameraPos += cameraSpeed * cameraUp;
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraUp;
//}
//
//int main() {
//    // Create a GLFW window with a width of 800 and height of 600, titled "BattleBeyz"
//    GLFWwindow* window = initGLFWandGLEW("Battlebeyz", 800, 600);
//    int windowWidth, windowHeight;
//
//    ShaderProgram shaderProgram(vertexShaderSource, fragmentShaderSource);
//    ShaderProgram textShaderProgram(textVertexShaderSource, textFragmentShaderSource);
//
//    // Register inputs here
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//
//    // VBO = Vertex Buffer Object, stores texture data: Positions, Normals, Texture Coordinates
//    // VBA = Vertex Array Object, stores attribute settings and used VBO
//    GLuint triangleVBO, triangleVAO, triangleEBO;
//    // Set up vertex data for a triangle and configure vertex attributes
//    float vertices[] = {
//            // Positions         // Colors
//            -0.4f, -0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // Bottom left, red
//            0.4f, -0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  // Bottom right, green
//            0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f   // Top, blue
//    };
//
//    setupBuffers(triangleVAO, triangleVBO, vertices, sizeof(vertices));
//
//    float floorVertices[] = {
//            -10.0f, 0.0f, -10.0f, 0.5f, 0.5f, 0.5f,
//            10.0f, 0.0f, -10.0f, 0.5f, 0.5f, 0.5f,
//            10.0f, 0.0f, 10.0f, 0.5f, 0.5f, 0.5f,
//            -10.0f, 0.0f, 10.0f, 0.5f, 0.5f, 0.5f,
//    };
//
//    unsigned int floorIndices[] = {
//            0, 1, 2,
//            2, 3, 0
//    };
//
//    GLuint floorVAO, floorVBO, floorEBO;
//
//    setupBuffers(floorVAO, floorVBO, floorEBO, floorVertices, sizeof(floorVertices), floorIndices, sizeof(floorIndices));
//
//    // Initialize font rendering
//    TextRenderer textRenderer("../assets/fonts/MetalFight.ttf", 800, 600);
//
//    // Initial matrices for model, view, clip
//    auto model = glm::mat4(1.0f);
//    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
//
//    shaderProgram.setUniforms(model, view, projection);
//
//    // Initial matrices for model, view, clip
//    auto textModel = glm::mat4(1.0f);
//    glm::mat4 textView = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    glm::mat4 textProjection = glm::ortho(0.0f, static_cast<GLfloat>(windowWidth), 0.0f, static_cast<GLfloat>(windowHeight));
//
//    textShaderProgram.setUniforms(textModel, textView, textProjection);
//
//    // Render loop
//    while (!glfwWindowShouldClose(window)) {
//        // Process input (keyboard, mouse, etc.)
//        processInput(window);
//
//        // Clear the color and depth buffers to prepare for a new frame
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        shaderProgram.use();
//
//        // Update the view matrix based on the current camera position and orientation
//        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//        shaderProgram.setUniforms(model, view, projection);
//
//        // Use the shader program and bind the VAO and draw objects
//        glBindVertexArray(triangleVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 3);
//        glBindVertexArray(floorVAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
//
//        // Update window dimensions
//        textShaderProgram.use();
//        glUniformMatrix4fv(glGetUniformLocation(textShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(textProjection));
//
//        glfwGetWindowSize(window, &windowWidth, &windowHeight);
//        textRenderer.RenderText("Hello World", windowWidth-20.0f, windowHeight-20.0f, 5.0f, glm::vec3(0.5f, 0.8f, 0.2f));
//
//        // Swap the front and back buffers to display the rendered frame
//        glfwSwapBuffers(window);
//        // Poll for and process events (like input)
//        glfwPollEvents();
//    }
//
//    // Clean up resources (shaderProgram has destructor)
//    glDeleteVertexArrays(1, &triangleVAO);
//    glDeleteBuffers(1, &triangleVBO);
//    glDeleteVertexArrays(1, &floorVAO);
//    glDeleteBuffers(1, &floorVBO);
//    glDeleteBuffers(1, &floorEBO);
//
//    // Terminate GLFW to clean up allocated resources
//    glfwTerminate();
//    return 0;
//}
//
