#include "Stadium.h"

Stadium::Stadium(unsigned int vao, unsigned int vbo, unsigned int ebo, const glm::vec3 &pos, const glm::vec3 &col,
                 const glm::vec3 &ringColor, const glm::vec3& crossColor, float radius, float curvature, int numRings,
                 int verticesPerRing, Texture* texture, float textureScale, PhysicsWorld* physicsWorld)
        : GameObject(vao, vbo, ebo, pos, col), ringColor(ringColor), crossColor(crossColor), radius(radius), curvature(curvature),
          numRings(numRings), verticesPerRing(verticesPerRing), texture(texture), textureScale(textureScale), physicsWorld(physicsWorld) {
    body = new ImmovableRigidBody(pos, glm::vec3(radius * 2.0f, curvature * radius * radius, radius * 2.0f));
    physicsWorld->addBody(body);
    Stadium::initializeMesh();
    std::cout << "Stadium color: (" << color.x << ", " << color.y << ", " << color.z << ")\n";
}

void Stadium::generateMeshData() {

    // Clear existing data
    vertices.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
    tangents.clear();

    // Add center vertex first
    vertices.emplace_back(0, 0, 0);
    texCoords.emplace_back(0.5, 0.5);
    colors.emplace_back(crossColor);

    if(verticesPerRing % 4 != 0) {
        std::cerr << "Vertices per ring must be a multiple of 4" << std::endl;
        return;
    }

    // Generate vertices
    for (int rIdx = 1; rIdx <= numRings; ++rIdx) {
        float r = pow(static_cast<float>(rIdx) / numRings, 0.5) * radius;
        for (int thetaIdx = 0; thetaIdx < verticesPerRing; ++thetaIdx) {
            float theta = 2.0f * M_PI * static_cast<float>(thetaIdx) / static_cast<float>(verticesPerRing);
            float x = r * std::cos(theta);
            float z = r * std::sin(theta); // Change y to z
            float y = std::pow(r, 2.0f) * curvature; // Use r for y calculation

            vertices.emplace_back(x, y, z);
            texCoords.emplace_back(textureScale * (r / radius * std::cos(theta)) + 0.5f,
                                   textureScale * (r / radius * std::sin(theta)) + 0.5f);
            // Set ring color for middle and end
            if(rIdx == numRings / 4 || rIdx == numRings - 1) {
                colors.emplace_back(ringColor);
            } else if(thetaIdx == 0 || thetaIdx == verticesPerRing / 4 || thetaIdx == verticesPerRing / 2 || thetaIdx == 3 * verticesPerRing / 4) {
                colors.emplace_back(crossColor);
            } else {
                colors.emplace_back(color);
            }
//            // Create a spiral color pattern
//            if ((rIdx + thetaIdx * 2) % (verticesPerRing / 2) < verticesPerRing / (numRings / 2)) {
//                colors.emplace_back(ringColor);
//            } else if (thetaIdx % (verticesPerRing / 12) == 0) {
//                colors.emplace_back(crossColor);
//            } else {
//                colors.emplace_back(color);
//            }
        }
    }

    normals.resize(vertices.size(), glm::vec3(0.0f));
    tangents.resize(vertices.size(), glm::vec3(0.0f));

    // Draw from origin to first ring
    for (int i = 0; i < verticesPerRing; ++i) {
        int origin = 0;
        int curr = i + 1;
        int next = (i + 1) % verticesPerRing + 1;
        indices.push_back(origin);
        indices.push_back(curr);
        indices.push_back(next);
        glm::vec3 v0 = vertices[origin];
        glm::vec3 v1 = vertices[curr];
        glm::vec3 v2 = vertices[next];
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
        normals[origin] += normal;
        normals[curr] += normal;
        normals[next] += normal;
        // Calculate tangent for the triangle
        glm::vec2 uv0 = texCoords[origin];
        glm::vec2 uv1 = texCoords[curr];
        glm::vec2 uv2 = texCoords[next];
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;
        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        tangents[origin] += tangent;
        tangents[curr] += tangent;
        tangents[next] += tangent;
    }

    // Do pairs starting from (1, 2) to (n-1, n)
    for (int rIdx = 1; rIdx < numRings; ++rIdx) {
        int offset1 = (rIdx - 1) * verticesPerRing + 1;
        int offset2 = rIdx * verticesPerRing + 1;

        for (int vertIdx = 0; vertIdx < verticesPerRing; ++vertIdx) {
            int curr1 = offset1 + vertIdx;
            int next1 = offset1 + (vertIdx + 1) % verticesPerRing;
            int curr2 = offset2 + vertIdx;
            int next2 = offset2 + (vertIdx + 1) % verticesPerRing;

            // Triangle 1: (curr1, curr2, next1)
            indices.push_back(curr1);
            indices.push_back(curr2);
            indices.push_back(next1);

            // Calculate normal for Triangle 1
            glm::vec3 v0 = vertices[curr1];
            glm::vec3 v1 = vertices[curr2];
            glm::vec3 v2 = vertices[next1];
            glm::vec3 edge1 = v1 - v0;
            glm::vec3 edge2 = v2 - v0;
            glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

            normals[curr1] += normal;
            normals[curr2] += normal;
            normals[next1] += normal;

            // Calculate tangent for Triangle 1
            glm::vec2 uv0 = texCoords[curr1];
            glm::vec2 uv1 = texCoords[curr2];
            glm::vec2 uv2 = texCoords[next1];
            glm::vec2 deltaUV1 = uv1 - uv0;
            glm::vec2 deltaUV2 = uv2 - uv0;
            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
            tangents[curr1] += tangent;
            tangents[curr2] += tangent;
            tangents[next1] += tangent;

            // Triangle 2: (next1, curr2, next2)
            indices.push_back(next1);
            indices.push_back(curr2);
            indices.push_back(next2);

            // Calculate normal for Triangle 2
            v0 = vertices[next1];
            v1 = vertices[curr2];
            v2 = vertices[next2];
            edge1 = v1 - v0;
            edge2 = v2 - v0;
            normal = glm::normalize(glm::cross(edge1, edge2));

            normals[next1] += normal;
            normals[curr2] += normal;
            normals[next2] += normal;

            // Calculate tangent for Triangle 2
            uv0 = texCoords[next1];
            uv1 = texCoords[curr2];
            uv2 = texCoords[next2];
            deltaUV1 = uv1 - uv0;
            deltaUV2 = uv2 - uv0;
            f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
            tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
            tangents[next1] += tangent;
            tangents[curr2] += tangent;
            tangents[next2] += tangent;
        }
    }

    for (auto &normal: normals) {
        normal = glm::normalize(normal);
    }
    for (auto &tangent: tangents) {
        tangent = glm::normalize(tangent);
    }

    // Calculate bounding boxes for each triangle
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v1 = vertices[indices[i]];
        glm::vec3 v2 = vertices[indices[i + 1]];
        glm::vec3 v3 = vertices[indices[i + 2]];

        auto bbox = new BoundingBox();
        bbox->update(v1, v2, v3);

        // Add the bounding box to the immovable rigid body
        body->boundingBoxes.push_back(bbox);
    }

//    // Print out the vertices
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
}

void Stadium::initializeMesh() {
    generateMeshData();
    if (vertices.size() != normals.size() || vertices.size() != texCoords.size()) {
        std::cerr << "Mesh data is inconsistent" << std::endl;
        std::cout << "Vertices: " << vertices.size() << ", Normals: " << normals.size() << ", TexCoords: "
                  << texCoords.size() << std::endl;
        return;
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

        // Texture coordinates
        vertexData.push_back(texCoords[i].x);
        vertexData.push_back(texCoords[i].y);

        // Color data
        vertexData.push_back(colors[i].x);
        vertexData.push_back(colors[i].y);
        vertexData.push_back(colors[i].z);
    }
    ::setupBuffers(VAO, VBO, EBO, vertexData.data(), vertexData.size() * sizeof(float), indices.data(),
                   indices.size() * sizeof(unsigned int));
}

// Does not need to take in lightColor and lightPos, as these should be same for all objects
void Stadium::render(ShaderProgram &shader, const glm::vec3 &lightColor,
                     const glm::vec3 &lightPos) {
    shader.use();

    if (texture) {
        glActiveTexture(GL_TEXTURE0); // Activate the texture unit first
        texture->use(); // Bind the texture to the active texture unit
        shader.setInt("texture1", 0); // Set the texture1 uniform to use texture unit 0
    }

    // Bind appropriate uniforms (model, view, projection matrices)
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    shader.setUniformMat4("model", model);
//    shader.setUniformVec3("viewPos", viewPos);
    shader.setUniformVec3("lightColor", lightColor);
    shader.setUniformVec3("lightPos", lightPos);
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