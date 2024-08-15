////////////////////////////////////////////////////////////////////////////////
// Beyblade.h -- Beyblade object include -- rz -- 2024-08-08
// Copyright (c) 2024, Ricky Zhang.
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ShaderProgram.h"
#include "Buffers.h"
#include "Texture.h"
#include "GameObject.h"
#include "RigidBody.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <memory>
#include <iomanip>

class Beyblade : public GameObject {
public:
    Beyblade(std::string  modelPath, uint32_t vao, uint32_t vbo, uint32_t ebo,
             const glm::vec3& col, RigidBody* rigidBody);
    ~Beyblade();

    void update(float deltaTime);
    void initializeMesh() override;
    void render(ShaderProgram& shader, const glm::vec3& lightColor, const glm::vec3& lightPos) override;

protected:

private:
    std::unordered_map<std::string, glm::vec3> materialColors;
    RigidBody* rigidBody;
    std::string modelPath;
    Texture* texture{};

    void loadModel(const std::string& path);
    void printDebugInfo();
};
