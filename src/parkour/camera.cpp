#include <parkour/camera.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
    : Position{position}, mWorldUp{worldUp}, mYaw{yaw}, mPitch{pitch},
      mFront(glm::vec3(0.0f, 0.0f, -1.0f))
{
    updateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += mFront * velocity;
    if (direction == BACKWARD)
        Position -= mFront * velocity;
    if (direction == LEFT)
        Position -= mRight * velocity;
    if (direction == RIGHT)
        Position += mRight * velocity;
    if (direction == UP)
        Position += glm::vec3(0.0f, 1.0f, 0.0f) * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainmPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    mYaw += xoffset;
    mPitch += yoffset;

    if (constrainmPitch)
    {
        if (mPitch > 89.0f)
            mPitch = 89.0f;
        if (mPitch < -89.0f)
            mPitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new mFront vector
    glm::vec3 front;
    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    mFront = glm::normalize(front);
    // also re-calculate the mRight and mUp vector
    mRight =
        glm::normalize(glm::cross(mFront, mWorldUp)); // normalize the vectors, because their length
                                                      // gets closer to 0 the more you look up or
                                                      // down which results in slower movement.
    mUp = glm::normalize(glm::cross(mRight, mFront));

    if (targetSet)
    {
        Position = target - tps.radius * mFront;
    }
}

void Camera::updateTarget(const glm::vec3 &position)
{
    target = position;
    targetSet = true;
    updateCameraVectors();
}
