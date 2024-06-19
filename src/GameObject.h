#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include "Buffers.h"

class GameObject {
public:
    GameObject(unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3& pos, const glm::vec3& col)
            : VAO(vao), VBO(vbo), EBO(ebo), position(pos), color(col) {}

    // Pure virtual functions
    virtual void update() = 0;
    virtual void initializeMesh() = 0;
    virtual void render(ShaderProgram& shader) = 0;
protected:
    // Mesh data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;  // For defining triangles
    std::vector<glm::vec3> tangents;    // For advanced shading

    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 color;

    virtual void generateMeshData() = 0; // Pure virtual function

    void setupBuffers();
};

void GameObject::setupBuffers() {
    // Convert vertices, normals, and texCoords to a single array for VBO
    std::vector<float> vertexData;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertexData.push_back(vertices[i].x);
        vertexData.push_back(vertices[i].y);
        vertexData.push_back(vertices[i].z);
        if (!normals.empty()) {
            vertexData.push_back(normals[i].x);
            vertexData.push_back(normals[i].y);
            vertexData.push_back(normals[i].z);
        }
        if (!texCoords.empty()) {
            vertexData.push_back(texCoords[i].x);
            vertexData.push_back(texCoords[i].y);
        }
    }

    // Decide which setupBuffers function to call based on whether indices are provided
    if (!indices.empty()) {
        ::setupBuffers(VAO, VBO, EBO, vertexData.data(), vertexData.size() * sizeof(float), indices.data(), indices.size() * sizeof(unsigned int));
    } else {
        ::setupBuffers(VAO, VBO, vertexData.data(), vertexData.size() * sizeof(float));
    }
}