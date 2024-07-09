#include "Beyblade.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

Beyblade::Beyblade(std::string modelPath, unsigned int vao, unsigned int vbo, unsigned int ebo,
                   const glm::vec3& pos, RigidBody* rigidBody)
        : modelPath(std::move(modelPath)), GameObject(vao, vbo, ebo, pos, glm::vec3(1.0)), rigidBody(rigidBody) {
    Beyblade::initializeMesh();
}

Beyblade::~Beyblade() {
    delete texture;
}

void Beyblade::initializeMesh() {
    loadModel(modelPath);

    if (vertices.size() != normals.size() || vertices.size() != texCoords.size()) {
        std::cerr << "Mesh data is inconsistent" << std::endl;
        std::cout << "Vertices: " << vertices.size() << ", Normals: " << normals.size() << ", TexCoords: "
                  << texCoords.size() << std::endl;
        return;
    }

    // Ensure colors vector has the same size as vertices if colors are not present
    if (colors.empty()) {
        colors.resize(vertices.size(), glm::vec3(1.0f, 1.0f, 1.0f));  // Default color to white
        std::cout << "SET TO ALL WHITE" << std::endl;
    }



    for (size_t i = 0; i < vertices.size(); ++i) {
        // Vertex positions
        vertexData.push_back(vertices[i].x);
        vertexData.push_back(vertices[i].y);
        vertexData.push_back(vertices[i].z);

        // Normal data
        vertexData.push_back(normals[i].x);
        vertexData.push_back(normals[i].y);
        vertexData.push_back(normals[i].z);

        // Texture coordinates (if available)
        if (!texCoords.empty()) {
            vertexData.push_back(texCoords[i].x);
            vertexData.push_back(texCoords[i].y);
        } else {
            vertexData.push_back(0.0f);  // Default texture coordinate
            vertexData.push_back(0.0f);
        }

        // Color data
        vertexData.push_back(colors[i].x);
        vertexData.push_back(colors[i].y);
        vertexData.push_back(colors[i].z);
    }

    setupBuffers(VAO, VBO, EBO, vertexData.data(), vertexData.size() * sizeof(float), indices.data(),
                 indices.size() * sizeof(unsigned int));
}


void Beyblade::render(ShaderProgram& shader, const glm::vec3& viewPos, const glm::vec3& lightColor, const glm::vec3& lightPos) {
    shader.use();

    if (texture) {
        glActiveTexture(GL_TEXTURE0);
        texture->use();
        shader.setInt("texture1", 0);
    } else {

    }

    glm::mat4 model = glm::translate(glm::mat4(1.0f), rigidBody->position);
    shader.setUniformMat4("model", model);
    shader.setUniformVec3("viewPos", viewPos);
    shader.setUniformVec3("lightColor", lightColor);
    shader.setUniformVec3("lightPos", lightPos);

    // NOTE: should always be white as default
    shader.setUniformVec3("objectColor", color);

//    std::cout << "Rendering color: (" << color.x << ", " << color.y << ", " << color.z << ")\n";

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }
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

    // Extract directory from the path
    std::string baseDir = path.substr(0, path.find_last_of("/\\"));
    if (baseDir.empty()) {
        baseDir = ".";
    }

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str(), baseDir.c_str(), true, true);

    if (!warn.empty()) {
        std::cout << "WARN::TINYOBJLOADER::" << warn << std::endl;
    }

    if (!err.empty()) {
        std::cerr << "ERROR::TINYOBJLOADER::" << err << std::endl;
    }

    if (!ret) {
        std::cerr << "ERROR::TINYOBJLOADER::Failed to load/parse .obj.\n";
        return;
    }

    // Clear previous data
    vertices.clear();
    normals.clear();
    texCoords.clear();
    tangents.clear();
    colors.clear();
    indices.clear();

    std::unordered_map<unsigned int, glm::vec3> vertexMap;
    std::unordered_map<unsigned int, glm::vec3> normalMap;
    std::unordered_map<unsigned int, glm::vec2> texCoordMap;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;

    // Extract vertices
    for (size_t v = 0; v < attrib.vertices.size() / 3; ++v) {
        vertexMap[v] = glm::vec3(attrib.vertices[3 * v + 0], attrib.vertices[3 * v + 1], attrib.vertices[3 * v + 2]);
    }

    // Extract normals
    for (size_t n = 0; n < attrib.normals.size() / 3; ++n) {
        normalMap[n] = glm::vec3(attrib.normals[3 * n + 0], attrib.normals[3 * n + 1], attrib.normals[3 * n + 2]);
    }

    // Extract texture coordinates
    for (size_t t = 0; t < attrib.texcoords.size() / 2; ++t) {
        texCoordMap[t] = glm::vec2(attrib.texcoords[2 * t + 0], attrib.texcoords[2 * t + 1]);
    }

    // Extract indices and assemble vertex data
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            glm::vec3 vertex = vertexMap[index.vertex_index];
            glm::vec3 normal = normalMap.count(index.normal_index) ? normalMap[index.normal_index] : glm::vec3(0.0f, 0.0f, 0.0f);
            glm::vec2 texCoord = texCoordMap.count(index.texcoord_index) ? texCoordMap[index.texcoord_index] : glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
            normals.push_back(normal);
            texCoords.push_back(texCoord);
            indices.push_back(indices.size());
        }
    }

    // Handle materials
    for (auto & material : materials) {
        std::cout << "Material name: " << material.name << ", Diffuse texture: " << material.diffuse_texname << std::endl;
        if (!material.diffuse_texname.empty()) {
            std::string texturePath = baseDir + "/" + material.diffuse_texname;
            std::cout << "Loading texture from path: " << texturePath << std::endl;
            textureMap[material.name] = std::make_unique<Texture>(texturePath.c_str(), "diffuse");
        } else {
            std::cout << "Material has no diffuse texture." << std::endl;
        }

        // Set colors to the diffuse color of the material
        glm::vec3 diffuseColor(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
        materialColors[material.name] = diffuseColor;
    }

    // Assign materials to vertices
    for (const auto& shape : shapes) {
        for (size_t i = 0; i < shape.mesh.indices.size(); ++i) {
            int materialID = shape.mesh.material_ids[i / 3];
            auto& material = materials[materialID];

            glm::vec3 diffuseColor = materialColors[material.name];
            colors.push_back(diffuseColor);

            if (!material.diffuse_texname.empty() && textureMap.count(material.name)) {
                texture = textureMap[material.name].get();
            } else {
                texture = nullptr;
            }
        }
    }

//        // Print out the vertices
//    std::cout << "Vertices: " << vertices.size() << std::endl;
//    for (const auto &vertex: vertices) {
//        std::cout << std::fixed << std::setprecision(2) << "(" << vertex.x << ", " << vertex.y << ", " << vertex.z
//                  << ") ";
//    }
//
//// Print out the normals
//    std::cout << "\nNormals: " << normals.size() << std::endl;
//    for (const auto &normal: normals) {
//        std::cout << std::fixed << std::setprecision(2) << "(" << normal.x << ", " << normal.y << ", " << normal.z
//                  << ") ";
//    }
//
//// Print out the texture coordinates
//    std::cout << "\nTexture Coordinates: " << texCoords.size() << std::endl;
//    for (const auto &texCoord: texCoords) {
//        std::cout << std::fixed << std::setprecision(2) << "(" << texCoord.x << ", " << texCoord.y << ") ";
//    }
//
//// Print out the indices
//    std::cout << "\nIndices: " << indices.size() << std::endl;
//    for (size_t i = 0; i < indices.size(); i += 3) {
//        std::cout << "Triangle: (" << indices[i] << ", " << indices[i + 1] << ", " << indices[i + 2] << ") ";
//    }
//    // Print out the tangents
//    std::cout << "\nTangents: " << tangents.size() << std::endl;
//    for (const auto &tangent: tangents) {
//        std::cout << std::fixed << std::setprecision(2) << "(" << tangent.x << ", " << tangent.y << ", " << tangent.z
//                  << ") ";
//    }

    std::cout << "Model loaded successfully with " << vertices.size() << " vertices and " << indices.size() << " indices." << std::endl;
}

