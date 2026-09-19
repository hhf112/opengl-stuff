#pragma once

#include "glm/ext/matrix_transform.hpp"
#include "parkour/camera.hpp"
#include "parkour/mesh.hpp"
#include "parkour/model.hpp"
#include <parkour/hitbox.hpp>

#include <glad/glad.h>
#include <parkour/const.hpp>

struct RenderOpts
{
    Shader *shader = nullptr;
    Shader *lineShaer = nullptr;
    GLenum format = GL_TRIANGLES;
    bool showBoundaries = false;
};

class Renderable
{
  public:
    glm::vec3 Position{0.0f};
    glm::vec3 Scale{glm::vec3(1.0f)};
    Camera camera;
    RenderOpts renderOpts;
    float movementSpeed = Game::ctrWalkSpeed;
    float mouseSensitivity = Game::mouseSens;

    // constructor
    Renderable(Model &asset, glm::vec3 position, glm::vec3 scale, float yaw, float pitch)
        : Position{position}, mYaw{yaw}, mPitch{pitch}, mModel{asset}, mBounds{asset.getBounds()}

    {
        camera.updateTarget(Position + glm::vec3(0.0f, Game::PlayerHeight, 0.0f));
        updateUnitVecsFromCamera();
    }

    void ProcessKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            translate(glm::vec3(0.0f, 0.0f, velocity));
        if (direction == BACKWARD)
            translate(glm::vec3(0.0f, 0.0f, -velocity));
        if (direction == LEFT)
            translate(glm::vec3(-velocity, 0.0f, 0.0f));
        if (direction == RIGHT)
            translate(glm::vec3(velocity, 0.0f, 0.0f));
        if (direction == UP)
            translate(glm::vec3(0.0f, velocity, 0.0f));

        camera.updateTarget(Position + glm::vec3(0.0f, Game::PlayerHeight, 0.0f));
    }

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
    {
        camera.ProcessMouseMovement(xoffset, yoffset, constrainPitch);
        updateUnitVecsFromCamera();
    }

    void updateUnitVecsFromCamera()
    {
        const glm::vec3 &front = camera.getFront();
        const glm::vec3 &right = camera.getRight();
        // up is always world up (i.e y axis)

        mFront = glm::vec3(front.x, 0.0f, front.z);
        mRight = glm::vec3(right.x, 0.0f, right.z);
    }

    glm::mat4 getTransform()
    {
        auto transform = glm::mat4(1.0f);
        transform = glm::translate(transform, Position);
        transform = glm::rotate(transform, glm::radians(mYaw), glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::scale(transform, Scale);

        return transform;
    }

    Renderable &translate(const glm::vec3 &vec)
    {
        Position += mRight * vec.x + mUp * vec.y + mFront * vec.z;
        camera.updateTarget(Position + glm::vec3(0.0f, Game::PlayerHeight, 0.0f));

        return *this;
    }

    Renderable &scaleBy(const glm::vec3 &factor)
    {
        Scale *= factor;
        return *this;
    }

    const std::vector<Mesh> &getLineMeshes()
    {
        return mLineMeshes;
    }
    Model &getModel()
    {
        return mModel;
    }

    Hitbox getBounds()
    {
        return mBounds;
    }

    float getHeight() const
    {
        return mBounds.getHeight();
    }

    float getWidth() const
    {
        return mBounds.getWidth();
    }
    float getDepth() const
    {
        return mBounds.getDepth();
    }

  private:
    std::vector<Mesh> mLineMeshes;
    glm::vec3 mFront;
    glm::vec3 mUp{0.0f, 1.0f, 0.0f};
    glm::vec3 mRight;
    Hitbox mBounds;
    Model &mModel;
    float mYaw = 0.0f;
    float mPitch = 0.0f;
};
