#ifndef CAMERA_H
#define CAMERA_H

#include "glm/ext/matrix_transform.hpp"
#include "parkour/const.hpp"
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP
};

class Camera
{
  public:
    glm::vec3 target;
    bool targetSet = false;
    struct sphere
    {
        float radius = 0.8 * Game::PlayerHeight;
    };
    sphere tps;

    glm::vec3 Position{0.0f, 0.0f, 0.0f};
    float MovementSpeed{Game::ctrWalkSpeed};
    float MouseSensitivity{Game::mouseSens};

    Camera() = default;
    // constructor with vectors
    Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        if (!targetSet)
            return glm::lookAt(Position, Position + mFront, mUp);
        else
            return glm::lookAt(Position, target, mUp);

    }

    // processes input received from any keyboard-like input system. Accepts input
    // parameter in the form of camera defined ENUM (to abstract it from windowing
    // systems)
    void ProcessKeyboard(CameraMovement direction, float deltaTime);

    // processes input received from a mouse input system. Expects the offset
    // value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    void updateCameraVectors();
    void updateTarget(const glm::vec3 &position);

    const glm::vec3 &getFront()
    {
        return mFront;
    }
    const glm::vec3 &getUp()
    {
        return mUp;
    }
    const glm::vec3 &getRight()
    {
        return mRight;
    }

    float getYaw()
    {
        return mYaw;
    }
    float getPitch()
    {
        return mPitch;
    }

  private:
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp{0.0f, 1.0f, 0.0f};
    float mYaw{-90.0f};
    float mPitch{0.0f};

    // calculates the front vector from the Camera's (updated) Euler Angles
};
#endif
