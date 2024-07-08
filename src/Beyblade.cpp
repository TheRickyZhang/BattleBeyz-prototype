#include "Beyblade.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Beyblade::Beyblade(const std::string& modelPath, unsigned int vao, unsigned int vbo, unsigned int ebo,
                   const glm::vec3& pos, const glm::vec3& col, RigidBody* rigidBody)
        : GameObject(vao, vbo, ebo, pos, col), rigidBody(rigidBody) {
    // Load model
    loadModel(modelPath);
}

Beyblade::~Beyblade() = default;

void Beyblade::initializeMesh() {
    // Initialization code for the mesh
}

void Beyblade::generateMeshData() {
    // This function can be left empty if model data is loaded using TinyOBJLoader
}

void Beyblade::render(ShaderProgram& shader, const glm::vec3& viewPos, const glm::vec3& lightColor, const glm::vec3& lightPos) {
    shader.use();
    glm::mat4 model = glm::translate(glm::mat4(1.0f), rigidBody->position);
    shader.setUniformMat4("model", model);
    shader.setUniformVec3("viewPos", viewPos);
    shader.setUniformVec3("lightColor", lightColor);
    shader.setUniformVec3("lightPos", lightPos);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Beyblade::update(float deltaTime) {
    // Update physics
    rigidBody->update(deltaTime);
}

void Beyblade::loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), nullptr, true, true);

    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!ret) {
        std::cerr << "Failed to load/parse .obj.\n";
        return;
    }

    // Process the loaded data
    vertices.clear();
    normals.clear();
    texCoords.clear();
    tangents.clear();
    colors.clear();
    indices.clear();

    // Extract vertices
    for (size_t v = 0; v < attrib.vertices.size() / 3; ++v) {
        vertices.push_back({
                                   attrib.vertices[3 * v + 0],
                                   attrib.vertices[3 * v + 1],
                                   attrib.vertices[3 * v + 2]
                           });
    }

    // Extract normals
    for (size_t n = 0; n < attrib.normals.size() / 3; ++n) {
        normals.push_back({
                                  attrib.normals[3 * n + 0],
                                  attrib.normals[3 * n + 1],
                                  attrib.normals[3 * n + 2]
                          });
    }

    // Extract texture coordinates
    for (size_t t = 0; t < attrib.texcoords.size() / 2; ++t) {
        texCoords.push_back({
                                    attrib.texcoords[2 * t + 0],
                                    attrib.texcoords[2 * t + 1]
                            });
    }

    // Extract indices
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            indices.push_back(index.vertex_index);
        }
    }
}
