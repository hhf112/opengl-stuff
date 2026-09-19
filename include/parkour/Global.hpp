#pragma once
#include "parkour/Renderable.hpp"
#include "parkour/const.hpp"
#include <parkour/Renderer.hpp>
#include <parkour/ResourceManager.hpp>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

struct KeyState
{
    bool isPressed = false;
    bool wasPressed = false;
};

class Global
{
  public:
    Renderable *currentSubject = nullptr;
    Camera *currentCamera = nullptr;

    Renderer renderer;
	ResourceManager resources;

    KeyState keys[GLFW_KEY_LAST + 1] = {false};

    float ScreenWidth = 1920;
    float ScreenHeight = 1080;
    float fov = glm::radians(120.0f);
    float near = 0.1f;
    float far = Game::FovHorizontal;
    float ar = (float)ScreenWidth / (float)ScreenHeight;

    glm::mat4 GetProjectionMatrice()
    {
        return glm::perspective(fov, ar, near, far);
    }
};

GLFWwindow *initGFLFWAndGLAD(float ScreenWidth, float ScreenHeight,
                             void (*framebuffer_size_callback)(GLFWwindow *window, int width,
                                                               int height),
                             void (*mouse_callback)(GLFWwindow *window, double xposIn,

                                                    double yposIn));

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window, float deltaTime);
