#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderProgram.h"
#include "Buffers.h"
#include <iostream>

class GameObject {
public:
    GameObject(unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3& pos, const glm::vec3& col)
            : VAO(vao), VBO(vbo), EBO(ebo), position(pos), color(col) {}

    // Pure virtual functions
    virtual void update() = 0;
    virtual void initializeMesh() = 0;
    virtual void render(ShaderProgram &shader, const glm::vec3 &viewPos, const glm::vec3 &lightColor, const glm::vec3 &lightPos) = 0;
protected:
    // Mesh data
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;  // For defining triangles
    std::vector<glm::vec3> tangents;    // For advanced shading
    std::vector<glm::vec3> colors;

    std::vector<float> vertexData;

    unsigned int VAO, VBO, EBO;
    glm::vec3 position;
    glm::vec3 color;

    virtual void generateMeshData() = 0; // Pure virtual function
};