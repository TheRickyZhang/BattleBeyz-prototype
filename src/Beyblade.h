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

class Beyblade : public GameObject {
public:
    Beyblade(const std::string& modelPath, unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3& pos,
             const glm::vec3& col, RigidBody* rigidBody);
    ~Beyblade();

    void update(float deltaTime);
    void initializeMesh() override;
    void render(ShaderProgram& shader, const glm::vec3& viewPos, const glm::vec3& lightColor, const glm::vec3& lightPos) override;

protected:
    void generateMeshData() override;

private:
    RigidBody* rigidBody;

    void loadModel(const std::string& path);
};