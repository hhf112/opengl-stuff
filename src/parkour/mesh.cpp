#include <parkour/mesh.hpp>
#include <string>

Mesh::Mesh(std::vector<Vertex> _vertices, std::vector<unsigned int> _indices,
           std::vector<Texture> _textures)
    : vertices{_vertices}, indices{_indices}, textures{_textures}
{
    setupMesh();
}

void Mesh::Draw(Shader &shader, GLenum format) const
{
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string name = textures[i].type;
        glUniform1i(glGetUniformLocation(shader.ID, name.c_str()), i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(format, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],
                 GL_STATIC_DRAW);

    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords));
    // vertex tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Tangent));
    // vertex bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Bitangent));
    // color
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 3, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, Color));
    // ids
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void *)offsetof(Vertex, m_BoneIDs));

    // weights
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, m_Weights));
    glBindVertexArray(0);
}
