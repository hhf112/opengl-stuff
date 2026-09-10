#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "parkour/camera.hpp"
#include "parkour/mesh.hpp"
#include "parkour/model.hpp"
#include <parkour/hitbox.hpp>

#include <glad/glad.h>

#include <memory>

struct RenderOpts
{
    Shader *shader = nullptr;
    glm::mat4 view;
    glm::mat4 projection;
    GLenum format = GL_TRIANGLES;
    bool show_boundaries = false;
};

class Renderable
{
  public:
    RenderOpts renderOpts;
    float movementSpeed;

    Renderable(std::shared_ptr<Model> asset, glm::mat4 transform = glm::mat4(1.0))
        : mModel{asset}, mTransform{transform}
    {

        mBoundaries = mModel->mBounds;
    }

    Renderable(const std::string &path, glm::mat4 transform = glm::mat4(1.0))
        : mTransform{transform}
    {
        mModel = std::make_shared<Model>(path);
        mBoundaries = mModel->mBounds;
    }

    void BindCamera(Camera *camera_ptr)
    {
        camera = camera_ptr;
        camera->Position = mBoundaries.center + glm::vec3(0.0f, mBoundaries.getHeight() / 2, 1.0f);
    }

    const glm::mat4 &getTransform()
    {
        return mTransform;
    }

    Renderable &rotate(float angle, const glm::vec3 &axis)
    {
        auto rotation = glm::rotate(glm::mat4(1.0), angle, axis);
        mTransform = rotation * mTransform;
        mBoundaries.center = rotation * glm::vec4(mBoundaries.center, 100.0f);
        return *this;
    }

    Renderable &translate(const glm::vec3 &mag)
    {
        auto translation = glm::translate(glm::mat4(1.0), mag);
        mTransform = translation * mTransform;
        // camera->Position = translation * glm::vec4(camera->Position, 1.0f);
        mBoundaries.center = translation * glm::vec4(mBoundaries.center, 1.0f);
        return *this;
    }
    Renderable &scale(const glm::vec3 &factor)
    {

        auto scale = glm::scale(glm::mat4(1.0), factor);
        mTransform = scale * mTransform;
        mBoundaries.center = scale * glm::vec4(mBoundaries.center, 1.0f);
        return *this;
    }
    std::vector<Mesh> &getLineMeshes()
    {
        return mLineMeshes;
    }

    std::shared_ptr<Model> getModel()
    {
        return mModel;
    }

    const Hitbox &getBounds()
    {
        return mBoundaries;
    }

  private:
    Hitbox mBoundaries;
    Camera *camera;
    glm::mat4 mTransform;
    std::vector<Mesh> mLineMeshes;
    std::shared_ptr<Model> mModel;
};
