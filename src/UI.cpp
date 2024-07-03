#include "UI.h"

void showHomeScreen(GLFWwindow* window, Texture& homeScreenTexture) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    auto windowWidth = data->windowWidth;
    auto windowHeight = data->windowHeight;
    int img_width = 400;
    int img_height = 400;

    glBindTexture(GL_TEXTURE_2D, homeScreenTexture.ID);

    ImGui::SetNextWindowSize(ImVec2(*windowWidth, *windowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Home Screen", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // Centering helper
    float window_center_x = *windowWidth / 2.0f;

    // Center the text at the top
    ImGui::SetCursorPos(ImVec2(window_center_x - ImGui::CalcTextSize("Welcome to BattleBeyz!").x / 2.0f, 20));
    ImGui::Text("Welcome to BattleBeyz!");

    // Center the image below the text
    if (homeScreenTexture.ID != 0) {
        ImGui::SetCursorPos(ImVec2(window_center_x - img_width / 2.0f, 60));
        ImGui::Image((void*)(intptr_t)homeScreenTexture.ID, ImVec2(img_width, img_height));
    }

    // Center the buttons below the image
    float button_start_y = 60 + img_height + 20;
    ImGui::SetCursorPos(ImVec2(window_center_x - 75, button_start_y));
    if (ImGui::Button("Start Game", ImVec2(150, 30))) {
        data->showHomeScreen = false;
        data->showInfoScreen = true;
    }

    ImGui::SetCursorPos(ImVec2(window_center_x - 75, button_start_y + 40));
    if (ImGui::Button("Customize Beyblade", ImVec2(150, 30))) {
        data->showCustomizeScreen = true;
    }

    ImGui::SetCursorPos(ImVec2(window_center_x - 75, button_start_y + 80));
    if (ImGui::Button("About", ImVec2(150, 30))) {
        data->showAboutScreen = true;
    }

    ImGui::End();
}

void showInfoScreen(GLFWwindow* window, float (*imguiColor)[3]) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

    // Dynamically calculate the required window size based on contents
    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("BattleBeyz", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    // Display text
    ImGui::Text("Your Beyz");

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
    ImGui::ColorEdit3("background color", *imguiColor);

    // Plot histograms
    float arr[] = {0.64f, 0.51f, 0.52f, 0.43f, 0.49f, 0.56f};
    ImGui::PlotHistogram("Weights", arr, IM_ARRAYSIZE(arr), 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));

    // Add Menu button to go back to the Home Screen
    if (ImGui::Button("Menu")) {
        data->showHomeScreen = true;
        data->showInfoScreen = false;
    }

    ImGui::End();
}

void showCustomizeScreen(GLFWwindow* window, Texture& backgroundTexture) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    auto windowWidth = data->windowWidth;
    auto windowHeight = data->windowHeight;

    // Centering helper
    float window_center_x = *windowWidth / 2.0f;

    ImGui::SetNextWindowSize(ImVec2(*windowWidth, *windowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Customize Screen", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // Center the text at the top
    ImGui::SetCursorPos(ImVec2(window_center_x - ImGui::CalcTextSize("Customize Your Beyblade!").x / 2.0f, 20));
    ImGui::Text("Customize Your Beyblade!");

    // Center the buttons below the image
    float button_start_y = 80;
    ImGui::SetCursorPos(ImVec2(window_center_x - 75, button_start_y));
    if (ImGui::Button("Save Customization", ImVec2(150, 30))) {
        // Add logic to save customization
    }

    ImGui::SetCursorPos(ImVec2(window_center_x - 75, button_start_y + 40));
    if (ImGui::Button("Back to Home", ImVec2(150, 30))) {
        data->showCustomizeScreen = false;
    }

    ImGui::End();
}


void showAboutScreen(GLFWwindow* window, Texture& backgroundTexture) {
    auto* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
    auto windowWidth = data->windowWidth;
    auto windowHeight = data->windowHeight;

    // Centering helper
    float window_center_x = *windowWidth / 2.0f;

    ImGui::SetNextWindowSize(ImVec2(*windowWidth, *windowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("About Screen", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // Center the text at the top
    ImGui::SetCursorPos(ImVec2(window_center_x - ImGui::CalcTextSize("About BattleBeyz").x / 2.0f, 20));
    ImGui::Text("About BattleBeyz");

    // Center the buttons below the image
    float button_start_y = 80;
    ImGui::SetCursorPos(ImVec2(window_center_x - 75, button_start_y));
    if (ImGui::Button("Back to Home", ImVec2(150, 30))) {
        data->showAboutScreen = false;
    }

    ImGui::End();
}

