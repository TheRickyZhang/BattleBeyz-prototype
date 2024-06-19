#pragma once

#include "GameObject.h"

class Stadium : public GameObject {
public:
    Stadium(unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3& pos,
            const glm::vec3& col, float radius, float curvature, float meshQuality);

    void update() override {}
    void initializeMesh() override;
    void render(ShaderProgram& shader) override;

protected:
    void generateMeshData() override;

private:
    float radius;
    float curvature;
    float meshQuality;
};