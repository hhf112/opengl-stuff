#pragma once

#ifndef MESH_H
#define MESH_H

#include <parkour/hitbox.hpp>
#include <parkour/shader.hpp>
#include <parkour/util.hpp>

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#define MAX_BONE_INFLUENCE 4

class Mesh
{
  public:
    Mesh() = default;
    // mesh Data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO;

    // constructor
    Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices,
         std::vector<Texture> _textures = std::vector<Texture>());

    // render the mesh
    void Draw(Shader &shader, GLenum format = GL_TRIANGLES) const;

  private:
    // render data
    unsigned int VBO, EBO;

    // initializes all the buffer objects/arrays
    void setupMesh();
};

namespace HitboxMeshConstructor
{
Mesh createMeshFromHitbox(Hitbox &hitbox);
}; // namespace HitboxMeshConstructor
#endif
