// #define DEBUG
#include <assimp/postprocess.h>
#include <parkour/model.hpp>

#include <iostream>

#include "assimp/material.h"
#include "parkour/util.hpp"

#define MESH_INFO                                                                                  \
    std::cerr << "<mesh index>\n";                                                                 \
    std::cerr << "found bones: " << std::boolalpha << mesh->HasBones() << '\n';                    \
    std::cerr << "diffuse textures: " << material->GetTextureCount(aiTextureType_DIFFUSE) << '\n'; \
    std::cerr << "base colors: " << material->GetTextureCount(aiTextureType_BASE_COLOR) << '\n';   \
    aiColor3D color(0.f, 0.f, 0.f);                                                                \
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)                               \
        std::cerr << "diffuse color: " << color.r << ", " << color.g << ", " << color.b << '\n';   \
    aiColor4D baseColor;                                                                           \
    if (material->Get(AI_MATKEY_BASE_COLOR, baseColor) == AI_SUCCESS)                              \
        std::cerr << "base color factor: " << baseColor.r << ", " << baseColor.g << ", "           \
                  << baseColor.b << ", " << baseColor.a << '\n';                                   \
    std::cerr << "found vertex colors: " << mesh->HasVertexColors(0) << std::boolalpha << "\n\n";

unsigned int Model::textureFromEmbedding(const aiScene *scene, unsigned int index)
{
    aiTexture *assimpTexture = scene->mTextures[index];

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (assimpTexture->mHeight == 0)
    {
        int width, height, nrComponents;
        unsigned char *data =
            stbi_load_from_memory(reinterpret_cast<unsigned char *>(assimpTexture->pcData),
                                  assimpTexture->mWidth, &width, &height, &nrComponents, 0);

        if (data)
        {
            GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE,
                         data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, assimpTexture->mWidth, assimpTexture->mHeight, 0,
                     GL_BGRA, GL_UNSIGNED_BYTE, assimpTexture->pcData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    std::cerr << "model.h: texture loaded from embedding\n";

    return textureID;
}

unsigned int Model::textureFromFile(const char *path, const std::string &directory, bool gamma)
{
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;

        switch (nrComponents)
        {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            stbi_image_free(data);
            std::cerr << "found " << nrComponents << " components.\n";
            std::cerr << "Unsupported number of image channels.\n";
            return -1;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
#ifdef DEBUG
        std::cerr << "Texture loaded from " << filename << '\n';
#endif
    }
    else
    {
        std::cout << "ERROR: Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

std::vector<Texture> Model::loadMaterialTextures(const aiScene *scene, aiMaterial *mat,
                                                 aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string path = str.C_Str();

        if (path[0] == '*')
        {
            unsigned int textureIndex = std::stoi(&str.C_Str()[1]);
            Texture texture;
            texture.id = textureFromEmbedding(scene, textureIndex);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
        }
        else
        {
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++)
            {
                if (textures_loaded[j].path.data() == path)
                {
                    textures.push_back(textures_loaded[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip)
            {
                Texture texture;
                texture.id = textureFromFile(str.C_Str(), this->directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture);
            }
        }
    }

    return textures;
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene, const glm::mat4 &transform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    Vertex vertex;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertex.Position =
            transform * glm::vec4(AssimpGLMHelpers::aiVec3ToGLM(mesh->mVertices[i]), 1.0f);

        // post transform
        mBounds.min = glm::min(mBounds.min, vertex.Position);
        mBounds.max = glm::max(mBounds.max, vertex.Position);

        // normals
        if (mesh->HasNormals())
            vertex.Normal = AssimpGLMHelpers::aiVec3ToGLM(mesh->mNormals[i]);

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vertex.TexCoords = AssimpGLMHelpers::aiVec3ToGLM(mesh->mTextureCoords[0][i]);
            // tangent
            vertex.Tangent = AssimpGLMHelpers::aiVec3ToGLM(mesh->mTangents[i]);
            // bitangent
            vertex.Bitangent = AssimpGLMHelpers::aiVec3ToGLM(mesh->mBitangents[i]);
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> diffuseMaps =
        loadMaterialTextures(scene, material, aiTextureType_DIFFUSE, "diffuseMap");

    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps =
        loadMaterialTextures(scene, material, aiTextureType_SPECULAR, "specularMap");

    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    std::vector<Texture> normalMaps =
        loadMaterialTextures(scene, material, aiTextureType_HEIGHT, "normalMap");

    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    std::vector<Texture> heightMaps =
        loadMaterialTextures(scene, material, aiTextureType_AMBIENT, "heightMap");

    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

#ifdef DEBUG
    MESH_INFO
#endif

    return Mesh(vertices, indices, textures);
}

void Model::processNode(aiNode *node, const aiScene *scene, const glm::mat4 &parentTransform)
{
    glm::mat4 currentTransform =
        parentTransform * AssimpGLMHelpers::AiMatrixToGlm(node->mTransformation);
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        mMeshes.emplace_back(processMesh(mesh, scene, currentTransform));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, currentTransform);
    }
}

void Model::loadModel(std::string const &path)
{
    Assimp::Importer importer;
    importer.SetPropertyBool(AI_CONFIG_PP_PTV_KEEP_HIERARCHY, true);
    const aiScene *scene = importer.ReadFile(
        path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_TransformUVCoords |
                  aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    std::cerr << "model loaded from: " << path << '\n';
    std::cerr << "found animations: " << std::boolalpha << scene->HasAnimations() << '\n';
    std::cerr << '\n';

    processNode(scene->mRootNode, scene, glm::mat4(1.0));
    mBounds = std::move(Hitbox(mBounds.min, mBounds.max));

    std::cerr << textures_loaded.size() << " texture(s) loaded.\n";
    std::cerr << mMeshes.size() << " mesh(s) processed.\n";
}
