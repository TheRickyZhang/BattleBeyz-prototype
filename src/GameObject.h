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

    void setupBuffers() {
        // Convert vertices, normals, texCoords, and tangents to a single array for VBO
        std::vector<float> vertexData;
        for (size_t i = 0; i < vertices.size(); ++i) {
            // Vertex positions
            vertexData.push_back(vertices[i].x);
            vertexData.push_back(vertices[i].y);
            vertexData.push_back(vertices[i].z);
            // Normals
            if (!normals.empty()) {
                vertexData.push_back(normals[i].x);
                vertexData.push_back(normals[i].y);
                vertexData.push_back(normals[i].z);
            }
            // Texture coordinates
            if (!texCoords.empty()) {
                vertexData.push_back(texCoords[i].x);
                vertexData.push_back(texCoords[i].y);
            }
            // Tangents
            if (!tangents.empty()) {
                vertexData.push_back(tangents[i].x);
                vertexData.push_back(tangents[i].y);
                vertexData.push_back(tangents[i].z);
            }
        }

        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Generate and bind VBO
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

        // Generate and bind EBO if indices are provided
        if (!indices.empty()) {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        }

        // Set vertex attribute pointers
        size_t stride = 3 * sizeof(float); // Position always present
        if (!normals.empty()) stride += 3 * sizeof(float);
        if (!texCoords.empty()) stride += 2 * sizeof(float);
        if (!tangents.empty()) stride += 3 * sizeof(float);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        size_t offset = 3 * sizeof(float);
        if (!normals.empty()) {
            // Normal attribute
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
            glEnableVertexAttribArray(1);
            offset += 3 * sizeof(float);
        }
        if (!texCoords.empty()) {
            // Texture Coord attribute
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
            glEnableVertexAttribArray(2);
            offset += 2 * sizeof(float);
        }
        if (!tangents.empty()) {
            // Tangent attribute
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
            glEnableVertexAttribArray(3);
        }

        // Unbind VAO
        glBindVertexArray(0);
    }
};