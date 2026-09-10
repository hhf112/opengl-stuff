#pragma once
#include "assimp/vector2.h"
#include "assimp/vector3.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define MAX_BONE_INFLUENCE 4
struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    // bone indexes which will influence this vertex
	glm::vec3 Color;
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
    unsigned int id;
    std::string type;
    std::string path;
};

namespace AssimpGLMHelpers
{
inline glm::mat4 AiMatrixToGlm(const aiMatrix4x4 &from)
{
    return glm::transpose(glm::make_mat4(&from.a1));
}

inline glm::vec3 aiVec3ToGLM(aiVector3D &vec)
{
    Vertex vertex;
    glm::vec3 vector;

    vector.x = vec.x;
    vector.y = vec.y;
    vector.z = vec.z;
    return vector;
}

inline glm::vec2 aiVec2toGLM(aiVector2D &vec)
{
    Vertex vertex;
    glm::vec2 vector;

    vector.x = vec.x;
    vector.y = vec.y;
    return vector;
}
} // namespace AssimpGLMHelpers
