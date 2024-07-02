#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ShaderProgram.h"
#include "TextRenderer.h"
#include "Texture.h"
#include "ShaderPath.h"

#include <iostream>
#include <sstream>
#include "Buffers.h"
#include "Stadium.h"
#include "Camera.h"
#include "Callbacks.h"
#include <iomanip>
#include <algorithm>

int main() {
    // "Global" variables

    // Window dimensions
    int windowWidth = 1600, windowHeight = 900;
    const float aspectRatio = 16.0f / 9.0f;
    int minWidth = windowWidth / 4;
    int minHeight = static_cast<int>(double(minWidth) / aspectRatio);

    // Relevant matrices
    auto model = glm::mat4(1.0f);
    auto view = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);

    // Primary camera and camera state
    Camera mainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    auto cameraState = new CameraState(&mainCamera, 400.0, 300.0);

    // Time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    static float imguiColor[3] = {1.0f, 0.0f, 0.0f}; // Red

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window. Note you NEED to make context current to initialize everything else
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "BattleBeyz", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

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

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Initialize ShaderProgram for 3D objects
    auto objectShader = new ShaderProgram(OBJECT_VERTEX_SHADER_PATH, OBJECT_FRAGMENT_SHADER_PATH);

    // Initialize font rendering
    TextRenderer textRenderer("../assets/fonts/paladins.ttf", 800, 600);

    // Initialize textures. Note that texture1 is primary texture
    Texture hexagonPattern("../assets/images/Hexagon.jpg", "texture1");
    Texture smallHexagonPattern("../assets/images/HexagonSmall.jpg", "texture1");
    std::cout << "Texture ID: " << hexagonPattern.ID << std::endl;
    std::cout << "Texture ID: " << smallHexagonPattern.ID << std::endl;

    // Initialize camera and camera state (uh oh, we're getting to double pointers now...)
    CallbackData callbackData(&windowWidth, &windowHeight, aspectRatio, &projection, objectShader, cameraState, true);

    // Store the callback data in the window for easy access
    glfwSetWindowUserPointer(window, &callbackData);

    // Handle resizing the window
    glfwSetWindowSizeLimits(window, minWidth, minHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Other callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    GLuint tetrahedronVAO, tetrahedronVBO, tetrahedronEBO;
    float tetrahedronVertices[] = {
            // Positions 0-2                // Normals 3-5                      // TexCoords  6-7          // Colors 8-10
            0.0f,  1.0f,  0.0f,  0.0f,  0.5773f,  0.0f,  0.5f, 1.0f,  1.0f, 0.0f, 0.0f, // Top vertex (Red)
            0.0f,  0.0f, -1.0f,  0.0f,  0.5773f, -0.8165f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Front vertex (Green)
            -1.0f,  0.0f,  1.0f, -0.8165f,  0.5773f,  0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // Left vertex (Blue)
            1.0f,  0.0f,  1.0f,  0.8165f,  0.5773f,  0.0f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f  // Right vertex (Yellow)
    };

    unsigned int tetrahedronIndices[] = {
            0, 1, 2, // Front face
            0, 1, 3, // Right face
            0, 2, 3, // Left face
            1, 2, 3  // Bottom face
    };
    setupBuffers(tetrahedronVAO, tetrahedronVBO, tetrahedronEBO, tetrahedronVertices,
                 sizeof(tetrahedronVertices), tetrahedronIndices, sizeof(tetrahedronIndices));

    // Initialize VAO, VBO, and EBO for the floor
    GLuint floorVAO, floorVBO, floorEBO;
    float floorVertices[] = {
            // Positions        // Normals       // TexCoords // Colors
            -10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,
            10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f, 0.5f, 0.5f, 0.5f,
            10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f, 0.5f, 0.5f, 0.5f,
            -10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f, 0.5f, 0.5f, 0.5f,
    };

    unsigned int floorIndices[] = {
            0, 1, 2,
            2, 3, 0
    };
    setupBuffers(floorVAO, floorVBO, floorEBO, floorVertices, sizeof(floorVertices), floorIndices, sizeof(floorIndices));

    // Initialize VAO, VBO, and EBO for the floor
    GLuint stadiumVAO = 0, stadiumVBO = 0, stadiumEBO = 0;
    // Create the Stadium object
    auto stadiumPosition = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 stadiumColor = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 ringColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 crossColor = glm::vec3(0.0f, 0.0f, 1.0f);
    float stadiumRadius = 4.0f;
    float stadiumCurvature = 0.02f;
    int numRings = 10;
    int sectionsPerRing = 64;
    float stadiumTextureScale = 1.5f;

    Stadium stadium(stadiumVAO, stadiumVBO, stadiumEBO, stadiumPosition, stadiumColor, ringColor, crossColor,
                    stadiumRadius, stadiumCurvature, numRings, sectionsPerRing, stadiumTextureScale);

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
        // Measure time
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Poll events at the start to process input before rendering
        glfwPollEvents();

        // Process input (keyboard, mouse, etc.)
        processInput(window, deltaTime);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show ImGui demo window if imguiActive is true
        if (callbackData.imguiActive) {
            ImGui::SetNextWindowSize(ImVec2(400, 200));
            // ImGui interface setup
            ImGui::Begin("BattleBeyz");
            // Display text
            ImGui::Text("Your Beyz", 123.456f);

            // Display more complex UI components
            static float f = 0.0f;
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            static int counter = 0;
            if (ImGui::Button("Launch")) {
                counter++;
            }
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            // Color editor
            ImGui::ColorEdit3("background color", imguiColor);
            // Plot histograms
            float arr[] = {0.64f, 0.51f, 0.52f, 0.43f, 0.49f, 0.56f};
            ImGui::PlotHistogram("Weights", arr, IM_ARRAYSIZE(arr), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));
            ImGui::End();
        }

        // Clear the color and depth buffers to prepare for a new frame
        glClearColor(imguiColor[0], imguiColor[1], imguiColor[2], 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader program
        objectShader->use();

        // Set light properties and view position
        objectShader->setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        objectShader->setUniformVec3("lightPos", glm::vec3(0.0f, 1e5f, 0.0f)); // Light position very high in the y-direction
        objectShader->setUniformVec3("viewPos", cameraState->camera->Position);

        // Update the view and projection matrices
        view = glm::lookAt(cameraState->camera->Position, cameraState->camera->Position + cameraState->camera->Front, cameraState->camera->Up);
        objectShader->setUniformMat4("view", view);
        objectShader->setUniformMat4("projection", projection);

        // Render the floor
        model = glm::mat4(1.0f);
        objectShader->setUniformMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        smallHexagonPattern.use();
        objectShader->setInt("texture1", 0);
        glBindVertexArray(floorVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // Render the tetrahedron
        model = glm::mat4(1.0f);
        objectShader->setUniformMat4("model", model);
        glActiveTexture(GL_TEXTURE0);
        hexagonPattern.use();
        objectShader->setInt("texture1", 0);
        glBindVertexArray(tetrahedronVAO);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

        // Update and render the stadium
        stadium.render(*objectShader, cameraState->camera->Position, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1e6f, 0.0f));

        // Render text overlay
        std::stringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "X: " << cameraState->camera->Position.x << " "
           << "Y: " << cameraState->camera->Position.y << " "
           << "Z: " << cameraState->camera->Position.z;
        std::string cameraPosStr = ss.str();
        std::replace(cameraPosStr.begin(), cameraPosStr.end(), '-', ';');

        // Render the camera position text
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        textRenderer.Resize(windowWidth, windowHeight);
        textRenderer.RenderText(cameraPosStr, 25.0f, windowHeight - 50.0f, 0.6f, glm::vec3(0.5f, 0.8f, 0.2f));

        // Render ImGui on top of the 3D scene
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers at the end
        glfwSwapBuffers(window);
    }

    // Cleanup OpenGL objects
    glDeleteVertexArrays(1, &tetrahedronVAO);
    glDeleteBuffers(1, &tetrahedronVBO);
    glDeleteBuffers(1, &tetrahedronEBO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &floorEBO);

    // Cleanup textures
    hexagonPattern.cleanup();
    smallHexagonPattern.cleanup();

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Destroy the GLFW window and terminate GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}