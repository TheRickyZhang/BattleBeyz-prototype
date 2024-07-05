#include "UI.h"

inline void CenterWrappedText(float window_center_x, float wrap_width, const char* text) {
    ImVec2 textSize = ImGui::CalcTextSize(text, text + strlen(text), false, wrap_width);
    ImGui::SetCursorPosX(window_center_x - textSize.x / 2.0f);
    ImGui::PushTextWrapPos(window_center_x + wrap_width / 2.0f);
    ImGui::TextWrapped(text);
    ImGui::PopTextWrapPos();
}

ImVec2 CalculateButtonSize(const char* text, float padding = 20.0f, float height = 30.0f) {
    ImVec2 textSize = ImGui::CalcTextSize(text);
    return ImVec2(textSize.x + padding, height);  // Add padding to the button width
}

inline bool CenterButton(float window_center_x, const char* label, float padding = 20.0f, float height = 30.0f) {
    ImVec2 buttonSize = CalculateButtonSize(label, padding, height);
    ImGui::SetCursorPosX(window_center_x - buttonSize.x / 2.0f);
    return ImGui::Button(label, buttonSize);
}

inline void TextWithLink(const char* text, const char* url) {
    ImVec4 linkColor = ImVec4(0.2f, 0.2f, 1.0f, 1.0f);  // Blue color
    ImGui::TextColored(linkColor, text);

    // Check if the item is hovered and clicked
    if (ImGui::IsItemHovered()) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (ImGui::IsMouseClicked(0)) {
            ShellExecute(nullptr, nullptr, url, nullptr, nullptr, SW_SHOW);
        }

        // Underline the text to indicate a hyperlink
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 cursorPos = ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y);
        ImGui::GetWindowDrawList()->AddLine(
                cursorPos,
                ImVec2(cursorPos.x + textSize.x, cursorPos.y),
                ImGui::GetColorU32(linkColor));
    }
}

void setupBackground(GLFWwindow *window, Texture &backgroundTexture) {
    auto *data = static_cast<CallbackData *>(glfwGetWindowUserPointer(window));
    auto windowWidth = *data->windowWidth;
    auto windowHeight = *data->windowHeight;
    auto backgroundShader = data->backgroundShader;
    auto quadRenderer = data->quadRenderer;

    if (backgroundTexture.ID != 0) {
        glm::mat4 ortho = glm::ortho(0.0f, (float) windowWidth, 0.0f, (float) windowHeight, -1.0f, 1.0f);
        backgroundShader->use();
        backgroundShader->setUniform1f("time", glfwGetTime());
        // Bind the background texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, backgroundTexture.ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        backgroundShader->setInt("backgroundTexture", 0);
        quadRenderer->render();
    }
}

// Note: shows image and texture
void showHomeScreen(GLFWwindow *window, Texture &homeScreenTexture, Texture &backgroundTexture) {
    auto *data = static_cast<CallbackData *>(glfwGetWindowUserPointer(window));
    auto windowWidth = data->windowWidth;
    auto windowHeight = data->windowHeight;
    int img_width = 400;
    int img_height = 400;

    setupBackground(window, backgroundTexture);

    glBindTexture(GL_TEXTURE_2D, homeScreenTexture.ID);

    // Calculate the size required to fit all elements plus some padding
    ImVec2 padding = ImVec2(50.0f, 50.0f);  // Padding around elements
    ImVec2 textSize = ImGui::CalcTextSize("Welcome to BattleBeyz!");
    ImVec2 imageSize = ImVec2(img_width, img_height);

    const char* buttonText[] = { "Start Game", "Customize Beyblade", "About" };
    int buttonCount = sizeof(buttonText) / sizeof(buttonText[0]);

    // Calculate the maximum width of the buttons
    float maxButtonWidth = 0.0f;
    for (int i = 0; i < buttonCount; i++) {
        ImVec2 buttonSize = CalculateButtonSize(buttonText[i]);
        if (buttonSize.x > maxButtonWidth) {
            maxButtonWidth = buttonSize.x;
        }
    }

    // Calculate total height required
    float totalHeight = textSize.y + 20 + imageSize.y + 20 + buttonCount * 40 + padding.y * 2;

    // Calculate total width required
    float totalWidth = std::max({ textSize.x, imageSize.x, maxButtonWidth }) + padding.x * 2;

    ImGui::SetNextWindowSize(ImVec2(totalWidth, totalHeight));
    ImGui::SetNextWindowPos(ImVec2((*windowWidth - totalWidth) / 2.0f, (*windowHeight - totalHeight) / 2.0f));
    ImGui::Begin("Home Screen", nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoTitleBar);

    // Centering helper
    float window_center_x = totalWidth / 2.0f;

    // Center the text at the top
    ImGui::SetCursorPos(ImVec2(window_center_x - textSize.x / 2.0f, padding.y));
    ImGui::Text("Welcome to BattleBeyz!");

    // Center the image below the text
    if (homeScreenTexture.ID != 0) {
        ImGui::SetCursorPos(ImVec2(window_center_x - img_width / 2.0f, padding.y + textSize.y + 20));
        ImGui::Image((void *) (intptr_t) homeScreenTexture.ID, ImVec2(img_width, img_height));
    }

    // Center the buttons below the image
    float button_start_y = padding.y + textSize.y + 20 + img_height + 20;

    for (int i = 0; i < buttonCount; i++) {
        ImVec2 buttonSize = CalculateButtonSize(buttonText[i]);

        ImGui::SetCursorPos(ImVec2(window_center_x - buttonSize.x / 2.0f, button_start_y + i * 40));
        if (ImGui::Button(buttonText[i], buttonSize)) {
            if (i == 0) {
                data->showHomeScreen = false;
                data->showInfoScreen = true;
            } else if (i == 1) {
                data->showCustomizeScreen = true;
            } else if (i == 2) {
                data->showAboutScreen = true;
            }
        }
    }

    ImGui::End();
}


void showInfoScreen(GLFWwindow *window, float (*imguiColor)[3]) {
    auto *data = static_cast<CallbackData *>(glfwGetWindowUserPointer(window));

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

    // Checkbox to toggle showCamera
    ImGui::Checkbox("Bound Camera", &data->boundCamera);

    // Add Menu button to go back to the Home Screen
    if (ImGui::Button("Menu")) {
        data->showHomeScreen = true;
        data->showInfoScreen = false;
    }

    ImGui::End();
}

void showCustomizeScreen(GLFWwindow *window, Texture &backgroundTexture) {
    auto *data = static_cast<CallbackData *>(glfwGetWindowUserPointer(window));
    auto windowWidth = data->windowWidth;
    auto windowHeight = data->windowHeight;

    setupBackground(window, backgroundTexture);

    // Centering helper
    float window_center_x = *windowWidth / 2.0f;

    ImGui::SetNextWindowSize(ImVec2(*windowWidth, *windowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Customize Screen", nullptr,
                 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                 ImGuiWindowFlags_NoTitleBar);

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
    auto windowWidth = float(*data->windowWidth);
    auto windowHeight = float(*data->windowHeight);
    auto defaultFont = data->defaultFont;
    auto titleFont = data->titleFont;

    setupBackground(window, backgroundTexture);

    // Calculate the center of the window for text alignment and set wrapping width to 60% of window width
    float window_center_x = windowWidth / 2.0f;
    float wrap_width = windowWidth * 0.7f;

    ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("About Screen", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // Apply larger font for the title and center it
    ImGui::PushFont(titleFont);
    CenterWrappedText(window_center_x, wrap_width, "About BattleBeyz");
    ImGui::PopFont();
    ImGui::Spacing();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    ImGui::PushFont(defaultFont);

    // Center the wrapped text blocks with a link
    {
        const char* part1 = "This project is currently in development. If you want to contribute, feel free to fork the repo";
        const char* linkText = "here";
        ImVec2 part1Size = ImGui::CalcTextSize(part1);
        ImVec2 linkSize = ImGui::CalcTextSize(linkText);
        float totalWidth = part1Size.x + linkSize.x;

        ImGui::SetCursorPosX(window_center_x - totalWidth / 2.0f);
        ImGui::TextUnformatted(part1);
        ImGui::SameLine();
        TextWithLink(linkText, "https://github.com/TheRickyZhang/BattleBeyz");
        ImGui::Spacing();
    }

    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    // Center the wrapped text blocks with another link
    {
        const char* part1 = "Like what you see? Show your support by getting some hand-crafted Lego Beyblades on";
        const char* linkText = "my website";
        ImVec2 part1Size = ImGui::CalcTextSize(part1);
        ImVec2 linkSize = ImGui::CalcTextSize(linkText);
        float totalWidth = part1Size.x + linkSize.x;

        ImGui::SetCursorPosX(window_center_x - totalWidth / 2.0f);
        ImGui::TextUnformatted(part1);
        ImGui::SameLine();
        TextWithLink(linkText, "https://www.brickbeyz.com/shop");
        ImGui::Spacing();
    }

    CenterWrappedText(window_center_x, wrap_width, "For any inquiries, please contact me at brickbeyz@outlook.com.");
    ImGui::Spacing();
    ImGui::Dummy(ImVec2(0.0f, 20.0f));

    CenterWrappedText(window_center_x, wrap_width, "Thank you for playing BattleBeyz!");
    ImGui::Spacing();

    CenterWrappedText(window_center_x, wrap_width, "(c) 2024 Brickbeyz. All rights reserved.");
    ImGui::PopFont();
    ImGui::Spacing();

    if (CenterButton(window_center_x, "Back to Home")) {
        data->showAboutScreen = false;
    }

    ImGui::End();
}
