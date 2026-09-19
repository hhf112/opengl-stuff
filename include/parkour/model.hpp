#ifndef MODEL_H
#define MODEL_H

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <parkour/hitbox.hpp>
#include <parkour/mesh.hpp>
#include <parkour/shader.hpp>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <nlohmann/json.hpp>

#include <string>
#include <vector>

#include "stb_image.h"

class Model
{
  public:
    std::vector<Texture> textures_loaded;
    bool gammaCorrection;

    Model() = default;
    // constructor, expects a filepath to a 3D model.
    Model(std::string const &path, bool gamma = false) : gammaCorrection(gamma)
    {
        loadModel(path);
    }

    Model(const std::vector<Mesh> &meshes) : mMeshes{std::move(meshes)}
    {
        calcBounds();
    }

    // draws the model, and thus all its mMeshes
    void Draw(Shader &shader, GLenum format = GL_TRIANGLES)
    {
        for (unsigned int i = 0; i < mMeshes.size(); i++)
        {
            mMeshes[i].Draw(shader, format);
        }
    }

    void calcBounds()
    {
        glm::vec3 min(std::numeric_limits<float>::max());
        glm::vec3 max(std::numeric_limits<float>::lowest());
        for (auto &mesh : mMeshes)
        {
            for (auto &vertice : mesh.vertices)
            {
                min = glm::min(min, vertice.Position);
                max = glm::max(max, vertice.Position);
            }
        }

        mBounds = std::move(Hitbox(min, max));
    }

    const Hitbox &getBounds()
    {
        return mBounds;
    }

    const std::string &getDirectory()
    {
        return directory;
    }

    const std::vector<Mesh> &getMeshes()
    {
        return mMeshes;
    }

  private:
    Hitbox mBounds;
    std::string directory;
    std::vector<Mesh> mMeshes;

    // loads a model with supported ASSIMP extensions from file and stores the
    // resulting mMeshes in the mMeshes std::vector.
    void loadModel(std::string const &path);

    // processes a node in a recursive fashion. Processes each individual mesh
    // located at the node and repeats this process on its children nodes (if
    // any).
    void processNode(aiNode *node, const aiScene *scene, const glm::mat4 &transform, glm::vec3 &min,
                     glm::vec3 &max);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform, glm::vec3 &min,
                     glm::vec3 &max);

    // checks all material textures of a given type and loads the textures if
    // they're not loaded yet. the required info is returned as a Texture struct.
    std::vector<Texture> loadMaterialTextures(const aiScene *scene, aiMaterial *mat,
                                              aiTextureType type, std::string typeName);

    unsigned int textureFromFile(const char *path, const std::string &directory,
                                 bool gamma = false);

    unsigned int textureFromEmbedding(const aiScene *scene, unsigned int index);
};

#endif
