#pragma once
#include <parkour/shader.hpp>
#include <parkour/util.hpp>

#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Hitbox
{

    glm::vec3 mMin{std::numeric_limits<float>::max()};
    glm::vec3 mMax{std::numeric_limits<float>::lowest()};
    glm::vec3 mCenter;
    float mWidth;
    float mHeight;
    float mDepth;

  public:
    Hitbox() = default;
    Hitbox(glm::vec3 &min, glm::vec3 &max, const glm::mat4 &transform = glm::mat4(1.0f))
        : mMin{min}, mMax{max}
    {
        mHeight = std::abs(mMax.y - mMin.y);
        mWidth = std::abs(mMax.x - mMin.x);
        mDepth = std::abs(mMax.z - mMin.z);
        mCenter = glm::vec3(mMax.x - mWidth / 2, mMax.y - mHeight / 2, mMax.z - mDepth / 2);
    }

    glm::vec3 getMin()
    {
        return mMin;
    }

    glm::vec3 getMax()
    {
        return mMax;
    }

    glm::vec3 getCenter()
    {
        return mCenter;
    }

    std::vector<Vertex> getVertices()
    {

        std::vector<Vertex> vertices;
        vertices.reserve(8);

        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMin.x, mMin.y, mMin.z), 1.0))); // 0: mMin, mMin, mMin
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMax.x, mMin.y, mMin.z), 1.0))); // 1: mMax, mMin, mMin
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMax.x, mMax.y, mMin.z), 1.0))); // 2: mMax, mMax, mMin
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMin.x, mMax.y, mMin.z), 1.0))); // 3: mMin, mMax, mMin
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMin.x, mMin.y, mMax.z), 1.0))); // 4: mMin, mMin, mMax
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMax.x, mMin.y, mMax.z), 1.0))); // 5: mMax, mMin, mMax
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMax.x, mMax.y, mMax.z), 1.0))); // 6: mMax, mMax, mMax
        vertices.emplace_back(
            Vertex(glm::vec4(glm::vec3(mMin.x, mMax.y, mMax.z), 1.0))); // 7: mMin, mMax, mMax

        return vertices;
    }

    void updateWithTransform(const glm::mat4 &transform)
    {
        mMin = transform * glm::vec4(mMin, 1.0);
        mMax = transform * glm::vec4(mMax, 1.0);
        mHeight = std::abs(mMax.y - mMin.y);
        mWidth = std::abs(mMax.x - mMin.x);
        mDepth = std::abs(mMax.z - mMin.z);
    }

    float getHeight() const
    {
        return mHeight;
    }

    float getWidth() const
    {
        return mWidth;
    }
    float getDepth() const
    {
        return mDepth;
    }
};
