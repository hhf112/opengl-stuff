#pragma once

#include "parkour/shader.hpp"
#include <parkour/model.hpp>
#include <unordered_map>

#define GENERIC_ERR_MSG                                                                            \
    {                                                                                              \
        std::cerr << "ERROR: invalid json format\n";                                               \
    }

class ResourceManager
{
  public:
    std::unordered_map<std::string, Model> models;
    std::unordered_map<std::string, Shader> shaders;
    std::unordered_map<std::string, Mesh> meshes;

    void loadFromJSON(const std::string &path)
    {
        /*
         *  parsing
         */
        nlohmann::json json;
        {
            std::fstream jsonFile{path, std::ios::in};
            if (!jsonFile)
            {
                std::cerr << "ERROR: invalid file path provided\n";
                return;
            }
            jsonFile >> json;
        }

        if (json["shaders"].is_array())
            loadShaders(json["shaders"]);
        if (json["models"].is_array())
            loadModels(json["models"]);
        if (json["meshes"].is_object())
            loadMeshes(json["meshes"]);
    }

    void loadShaders(nlohmann::json &json_arr)
    {
        if (json_arr.size() < 3)
            GENERIC_ERR_MSG

        for (auto start = json_arr.begin(), end = json_arr.end(); start != end;
             start = next(next(next(start))))
        {
            if (!start->is_null())
            {
                if (!start->is_string())
                    GENERIC_ERR_MSG

                // vertex and fragment shader required
                if (!next(start)->is_string() || !next(next(start))->is_string())
                    continue;

                shaders.insert(
                    std::make_pair(start->template get<std::string>().data(),
                                   Shader(next(start)->template get<std::string>().data(),
                                          next(next(start))->template get<std::string>().data())));
            }
        }
    }

    void loadModels(nlohmann::json &json_arr)
    {
        if (json_arr.size() < 2)
            GENERIC_ERR_MSG

        for (auto start = json_arr.begin(), end = json_arr.end(); start != end;
             start = next(next(start)))
        {
            if (!start->is_string() || !next(start)->is_string())
                GENERIC_ERR_MSG

            models[start->get<std::string>()] = Model(next(start)->get<std::string>());
        }
    }

    void loadMeshes(nlohmann::json &json_obj)
    {
        if (json_obj.is_object())
        {
            for (auto &[mesh_name, data] : json_obj.items())
            {

                std::vector<Vertex> vertices;
                if (data["vertices"].is_object())
                {

                    std::vector<glm::vec3> positions;
                    std::vector<glm::vec3> normals;
                    std::vector<glm::vec2> texcoords;
                    std::vector<glm::vec3> tangents;
                    std::vector<glm::vec3> bitangents;
                    std::vector<glm::vec3> colors;

                    auto &vertices_j = data["vertices"];
                    if (vertices_j["positions"].is_array())
                    {
                        auto positions_j = vertices_j["positions"].get<std::vector<float>>();
                        positions.resize(positions_j.size());
                        memcpy(positions.data(), positions_j.data(),
                               sizeof(float) * positions_j.size());
                    }

                    if (vertices_j["normals"].is_array())
                    {
                        auto normals_j = vertices_j["normals"].get<std::vector<float>>();
                        positions.resize(normals_j.size());
                        memcpy(normals.data(), normals_j.data(), sizeof(float) * normals_j.size());
                    }
                    if (vertices_j["texcoords"].is_array())
                    {
                        auto texcoords_j = vertices_j["texcoords"].get<std::vector<float>>();
                        positions.resize(texcoords_j.size());
                        memcpy(texcoords.data(), texcoords_j.data(),
                               sizeof(float) * texcoords_j.size());
                    }
                    if (vertices_j["tangents"].is_array())
                    {
                        auto tangents_j = vertices_j["tangents"].get<std::vector<float>>();
                        memcpy(tangents.data(), tangents_j.data(),
                               sizeof(float) * tangents_j.size());
                    }
                    if (vertices_j["bitangents"].is_array())
                    {
                        auto bitangents_j = vertices_j["bitangents"].get<std::vector<float>>();
                        memcpy(bitangents.data(), bitangents_j.data(),
                               sizeof(float) * bitangents_j.size());
                    }

                    if (vertices_j["colors"].is_array())
                    {
                        auto colors_j = vertices_j["colors"].get<std::vector<float>>();
						colors.resize(colors_j.size());
                        memcpy(colors.data(), colors_j.data(), sizeof(float) * colors_j.size());
                    }

                    for (int vertexIndex = 0; vertexIndex < vertices_j.size(); vertexIndex++)
                    {
                        Vertex vertex;
                        if (vertexIndex < positions.size())
                            vertex.Position = positions[vertexIndex];
                        if (vertexIndex < normals.size())
                            vertex.Normal = normals[vertexIndex];
                        if (vertexIndex < tangents.size())
                            vertex.Tangent = tangents[vertexIndex];
                        if (vertexIndex < bitangents.size())
                            vertex.Bitangent = bitangents[vertexIndex];
                        if (vertexIndex < texcoords.size())
                            vertex.TexCoords = texcoords[vertexIndex];
                        if (vertexIndex < colors.size())
                            vertex.Color = colors[vertexIndex];

                        vertices.emplace_back(vertex);
                    }
                }

                std::vector<unsigned int> indices;
                if (data["indices"].is_array())
                {
                    {
                        auto indices_j = data["indices"].get<std::vector<unsigned int>>();
                        indices.resize(indices_j.size());
                        memcpy(indices.data(), indices_j.data(),
                               sizeof(unsigned int) * indices_j.size());
                    }
                }

                meshes[mesh_name] = Mesh(vertices, indices);
            }
        }
    }
};
