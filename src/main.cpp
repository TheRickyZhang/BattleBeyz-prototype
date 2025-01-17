#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
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

#include "Buffers.h"
#include "Stadium.h"
#include "Camera.h"
#include "Callbacks.h"
#include "UI.h"
#include "QuadRenderer.h"
#include "Physics.h"


#include "PhysicsWorld.h"
#include "RigidBody.h"
#include "Beyblade.h"

#include <iomanip>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <atomic>

int main() {
    // Window dimensions
    int windowWidth = 1600, windowHeight = 900;
    const float aspectRatio = 16.0f / 9.0f;
    int minWidth = windowWidth / 4;
    int minHeight = static_cast<int>(double(minWidth) / aspectRatio);

    /* ----------------------INITIALIZATION-------------------------- */

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
        cleanup(window);
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        cleanup(window);
        return -1;
    }

    // Set color blinding and depth testing
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Initialize ImGui fonts
    ImFont* defaultFont = io.Fonts->AddFontFromFileTTF(DEFAULT_FONT_PATH, 24.0f);
    ImFont* titleFont = io.Fonts->AddFontFromFileTTF(TITLE_FONT_PATH, 48.0f);
    ImFont* attackFont = io.Fonts->AddFontFromFileTTF(ATTACK_FONT_PATH, 128.0f);
    io.Fonts->Build();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    /* ----------------------GLOBAL VARIABLES-------------------------- */

    // Relevant matrices
    auto model = glm::mat4(1.0f);
    auto view = glm::mat4(1.0f);
    auto projection = glm::mat4(1.0f);

    auto physicsWorld = new PhysicsWorld;

    // Primary camera and camera state
    glm::vec3 initialCameraPos(5.0f, 5.0f, 0.0f);
    glm::vec3 lookAtPoint(0.0f, 0.0f, 0.0f);
    glm::vec3 frontVector = glm::normalize(lookAtPoint - initialCameraPos);

// Calculate initial yaw and pitch
    float initialYaw = glm::degrees(std::atan2(frontVector.z, frontVector.x)); // Yaw based on X and Z
    float initialPitch = glm::degrees(std::atan2(frontVector.y, glm::length(glm::vec2(frontVector.x, frontVector.z))));

    Camera mainCamera(initialCameraPos, initialYaw, initialPitch, 0.0f, physicsWorld);
    mainCamera.Front = frontVector;  // Set the front vector to look at the desired point
    auto cameraState = new CameraState(&mainCamera, 400.0, 300.0);


    // Time variables
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    static float imguiColor[3] = {1.0f, 0.0f, 0.0f}; // Red


    /* ----------------------RELEVANT INSTANTIATIONS-------------------------- */


    auto quadRenderer = new QuadRenderer();

    auto identity4 = glm::mat4(1.0f);
    // Identity matrix, starting view, and projection matrices
    model = identity4;
    view = glm::lookAt(initialCameraPos, lookAtPoint, glm::vec3(0.0f, 1.0f, 0.0f));
    projection = glm::perspective(glm::radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.0f);

    // Set orthographic projection and depth values for the background shader
    auto backgroundModel = identity4;
    auto backgroundView = identity4;
    glm::mat4 orthoProjection = glm::ortho(0.0f, float(windowWidth), 0.0f, float(windowHeight), 0.0f, 1.0f);

//    // Position camera at the origin and rotate around it
//    auto panoramaModel = identity4;
//    glm::mat4 panoramaView = identity4;
//    glm::mat4 panoramaProjection = glm::perspective(glm::radians(45.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.0f);

    // Initialize ShaderProgram for 3D objects
    auto objectShader = new ShaderProgram(OBJECT_VERTEX_SHADER_PATH, OBJECT_FRAGMENT_SHADER_PATH);
    // Initialize default shader program with the model, view, and projection matrices. Also sets to use.
    objectShader->setUniforms(model, view, projection);
    objectShader->setUniformVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    objectShader->setUniformVec3("lightPos", glm::vec3(0.0f, 1e5f, 0.0f)); // Light position very high in the y-direction

    auto backgroundShader = new ShaderProgram(BACKGROUND_VERTEX_SHADER_PATH, BACKGROUND_FRAGMENT_SHADER_PATH);
    backgroundShader->setUniforms(backgroundModel, backgroundView, orthoProjection);
    backgroundShader->setUniform1f("wrapFactor", 4.0f);

//    auto panoramaShader = new ShaderProgram(PANORAMA_VERTEX_SHADER_PATH, PANORAMA_FRAGMENT_SHADER_PATH);
//    panoramaShader->setUniforms(panoramaModel, panoramaView, panoramaProjection);

    // Initialize font rendering
    TextRenderer textRenderer("../assets/fonts/paladins.ttf", 800, 600);

    // Initialize textures. Note that texture1 is primary texture
    Texture hexagonPattern("../assets/images/Hexagon.jpg", "texture1");
    Texture smallHexagonPattern("../assets/images/HexagonSmall.jpg", "texture1");
    auto floorTexture = new Texture("../assets/images/Wood1.jpg", "texture1");
    auto stadiumTexture = new Texture("../assets/images/Hexagon.jpg", "texture1");

    // Static texture object
    Texture homeScreenTexture("../assets/images/Brickbeyz.jpg", "texture1");
    Texture backgroundTexture("../assets/images/Brickbeyz.jpg", "texture1");
    std::cout << "Texture ID: " << hexagonPattern.ID << std::endl;
    std::cout << "Texture ID: " << smallHexagonPattern.ID << std::endl;
    std::cout << "Texture ID: " << homeScreenTexture.ID << std::endl;
    std::cout << "Texture ID: " << backgroundTexture.ID << std::endl;

    // Initialize camera and camera state
    CallbackData callbackData(&windowWidth, &windowHeight, aspectRatio, &projection,
                              objectShader, backgroundShader,cameraState, quadRenderer,
                              true, false, false, false, defaultFont,
                              titleFont, attackFont, false, ProgramState::ACTIVE);

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

    /* ----------------------OBJECT SETUP-------------------------- */

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
            -30.0f, 0.0f, -30.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,
            30.0f, 0.0f, -30.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f, 0.5f, 0.5f, 0.5f,
            30.0f, 0.0f,  30.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f, 0.5f, 0.5f, 0.5f,
            -30.0f, 0.0f,  30.0f, 0.0f, 1.0f, 0.0f, 0.0f, 4.0f, 0.5f, 0.5f, 0.5f,
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
    glm::vec3 stadiumColor = glm::vec3(0.2f, 0.2f, 0.2f);
    glm::vec3 ringColor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 crossColor = glm::vec3(0.0f, 0.0f, 1.0f);
    float stadiumRadius = 4.0f;
    float stadiumCurvature = 0.02f;
    int numRings = 10;
    int sectionsPerRing = 64;
    float stadiumTextureScale = 1.5f;

    Stadium stadium(stadiumVAO, stadiumVBO, stadiumEBO, stadiumPosition, stadiumColor, ringColor, crossColor,
                    stadiumRadius, stadiumCurvature, numRings, sectionsPerRing, stadiumTexture, stadiumTextureScale, physicsWorld);

    GLuint Bey1VAO = 0, Bey1VBO = 0, Bey1EBO = 0;
    auto rigidBey1 = new RigidBody(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(1.0f), 1.0f);
    std::string beyblade1Path = "../assets/images/beyblade.obj";
    auto bey1Position = glm::vec3(0.0f, 2.0f, 0.0f);
    Beyblade beyblade1(beyblade1Path, Bey1VAO, Bey1VBO, Bey1EBO, bey1Position, rigidBey1);

    /* ----------------------MAIN RENDERING LOOP-------------------------- */

    while (!glfwWindowShouldClose(window)) {
        // Measure time
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Poll events at the start to process input before rendering
        glfwPollEvents();

        // Process input (keyboard, mouse, etc.)
        processInput(window, deltaTime);

        // Update changing camera variables

        glm::vec3 cameraPos = cameraState->camera->Position;
        view = glm::lookAt(cameraState->camera->Position, cameraState->camera->Position + cameraState->camera->Front, cameraState->camera->Up);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        if (callbackData.currentState == ProgramState::LOADING) {
            showLoadingScreen(window, backgroundTexture);
        } else if (callbackData.showHomeScreen) {
            // Note: for better performance, distribute these changes to depth test when switching variables
            glDisable(GL_DEPTH_TEST);
            if(callbackData.showCustomizeScreen || callbackData.showAboutScreen) {
                if(callbackData.showCustomizeScreen) {
                    showCustomizeScreen(window, backgroundTexture);
                } else if(callbackData.showAboutScreen) {
                    showAboutScreen(window, backgroundTexture);
                }
            } else {
                showHomeScreen(window, homeScreenTexture, backgroundTexture);
            }
        } else {
            glEnable(GL_DEPTH_TEST);

            physicsWorld->update(deltaTime);

            if(callbackData.showInfoScreen) {
                showInfoScreen(window, &imguiColor);
            }

            // Clear the color and depth buffers to prepare for a new frame
            glClearColor(imguiColor[0], imguiColor[1], imguiColor[2], 1.00f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            // Use the shader program (objectShader) for rendering 3D objects, sets viewPos and view
            objectShader->use();
            objectShader->updateCameraPosition(cameraPos, view);
            objectShader->setUniformMat4("model", identity4);


            // Render the floor
            glActiveTexture(GL_TEXTURE0);
            smallHexagonPattern.use();
            glBindVertexArray(floorVAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            // Render the tetrahedron
            objectShader->setUniformMat4("model", identity4);
            glActiveTexture(GL_TEXTURE0);
            hexagonPattern.use();
            objectShader->setInt("texture1", 0);
            glBindVertexArray(tetrahedronVAO);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

            // Does not need to take in lightColor and lightPos, as these should be same for all objects

            // Update and render the stadium (uses this texture)
            stadium.render(*objectShader, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1e6f, 0.0f));

            // Update and render the Beyblade
            beyblade1.update(deltaTime);
            beyblade1.render(*objectShader, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 1e6f, 0.0f));

            // Render bounding boxes for debugging
            physicsWorld->renderDebug(*objectShader);
//            mainCamera.body->renderDebug(*objectShader, cameraState->camera->Position);
//            stadium.body->renderDebug(*objectShader, cameraState->camera->Position);


            // Render text overlay
            std::stringstream ss;
            ss << std::fixed << std::setprecision(1);
            ss << "X: " << cameraState->camera->Position.x << " "
               << "Y: " << cameraState->camera->Position.y << " "
               << "Z: " << cameraState->camera->Position.z << "   |   "
               << "Min" << cameraState->camera->body->boundingBoxes[0]->min.x << " " <<
                cameraState->camera->body->boundingBoxes[0]->min.y << " " <<
                cameraState->camera->body->boundingBoxes[0]->min.z << "\n"
                << "Max" << cameraState->camera->body->boundingBoxes[0]->max.x << " " <<
                cameraState->camera->body->boundingBoxes[0]->max.y << " " <<
                cameraState->camera->body->boundingBoxes[0]->max.z << "\n";
            std::string cameraPosStr = ss.str();
            std::replace(cameraPosStr.begin(), cameraPosStr.end(), '-', ';');

            // Render the camera position text
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            textRenderer.Resize(windowWidth, windowHeight);
            textRenderer.RenderText(cameraPosStr, 25.0f, windowHeight - 50.0f, 0.6f, glm::vec3(0.5f, 0.8f, 0.2f));
        }

        // Render ImGui on top of the 3D scene
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers at the end
        glfwSwapBuffers(window);
    }

    /* ----------------------CLEANUP-------------------------- */

    // Variables cleanup
    glDeleteVertexArrays(1, &tetrahedronVAO);
    glDeleteBuffers(1, &tetrahedronVBO);
    glDeleteBuffers(1, &tetrahedronEBO);
    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteBuffers(1, &floorEBO);

    // Clean up
    cleanup(window);

    // Cleanup textures
    hexagonPattern.cleanup();
    smallHexagonPattern.cleanup();

    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}