#pragma once
#include <parkour/shader.hpp>
#include <parkour/util.hpp>

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Hitbox
{
    float m_width;
    float m_height;
    float m_depth;

  public:
    glm::vec3 min{std::numeric_limits<float>::max()};
    glm::vec3 max{std::numeric_limits<float>::lowest()};
	glm::vec3 center;
    std::vector<Vertex> vertices;
    Hitbox() = default;

    Hitbox(glm::vec3 &_min, glm::vec3 &_max, const glm::mat4 &transform = glm::mat4(1.0f))
        : min{_min}, max{_max}
    {
        vertices.reserve(8);

        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(min.x, min.y, min.z), 1.0))); // 0: min, min, min
        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(max.x, min.y, min.z), 1.0))); // 1: max, min, min
        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(max.x, max.y, min.z), 1.0))); // 2: max, max, min
        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(min.x, max.y, min.z), 1.0))); // 3: min, max, min
        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(min.x, min.y, max.z), 1.0))); // 4: min, min, max
        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(max.x, min.y, max.z), 1.0))); // 5: max, min, max
        vertices.emplace_back(
            Vertex(transform * glm::vec4(glm::vec3(max.x, max.y, max.z), 1.0))); // 6: max, max, max
        vertices.emplace_back(Vertex(
            transform * glm::vec4(glm::vec3(min.x, max.y, max.z), 1.0))); // 7: min, max, max    }
        m_height = std::abs(max.y - min.y);
        m_width = std::abs(max.x - min.x);
        m_depth = std::abs(max.z - min.z);
		center = glm::vec3(max.x - m_width/2, max.y - m_height/2, max.z - m_depth/2);
    }

    float getHeight() const
    {
        return m_height;
    }

    float getWidth() const
    {
        return m_width;
    }
    float getDepth() const
    {
        return m_depth;
    }
};
