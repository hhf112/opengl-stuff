// #define DEBUG
#include "glm/trigonometric.hpp"
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <parkour/Renderable.hpp>
#include <parkour/Renderer.hpp>
#include <parkour/camera.hpp>
#include <parkour/model.hpp>
#include <parkour/shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <memory>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Renderer renderer;

    renderer.globalShaders.emplace_back("./shaders/vertex.vert", "./shaders/fragment.frag");
    renderer.globalShaders.emplace_back("./shaders/hitbox.vert", "./shaders/hitbox.frag");

    Renderable player("C:/Users/ASUS/OneDrive/Desktop/parkour/models/hu_tao_3d_model/scene.gltf");

    // camera presets
    const float PlayerHeight = 180.0f;
    const float FovHorizontal = 1000.0f;
    camera.fov = glm::radians(120.0f);
    camera.near = 0.1f;
    camera.far = FovHorizontal;
    camera.ar = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    camera.Position = glm::vec3(0.0f, 1.0 * PlayerHeight, 0.5 * PlayerHeight);
    camera.MovementSpeed = 100.0f;

    // floor
    const float quadSideL = 1.0f;
    std::vector<Vertex> quadV = {
        {{-quadSideL, -quadSideL, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 0.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 1.0f, 0.0f}}, // bottom-left
        {{-quadSideL, quadSideL, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {0.0f, 1.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 1.0f, 0.0f}}, // top-left
        {{quadSideL, quadSideL, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 1.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 1.0f, 0.0f}}, // top-right
        {{quadSideL, -quadSideL, 0.0f},
         {0.0f, 0.0f, 1.0f},
         {1.0f, 0.0f},
         {1.0f, 0.0f, 0.0f},
         {0.0f, 1.0f, 0.0f}}, // bottom-right
    };
    std::vector<unsigned int> quadInd = {0, 1, 2, 0, 2, 3};
    std::vector<Mesh> quadMeshes{Mesh(quadV, quadInd)};
    auto quadModel = std::make_shared<Model>(quadMeshes);
    Renderable quad(quadModel);

    // axes
    std::vector<glm::vec3> axesV = {
        {100.0f, 0.0f, 0.0f}, {0.0f, 100.0f, 0.0f}, {0.0f, 0.0f, 100.0f}, {0.0f, 0.0f, 0.0f}};
    std::vector<unsigned int> axesInd = {3, 0, 3, 1, 3, 2};
    std::vector<Vertex> axesVtx;
    for (int i = 0; i < axesV.size(); i++)
    {
        axesVtx.emplace_back(Vertex(axesV[i]));
    }
    axesVtx[0].Color = glm::vec3(1.0f, 0.0f, 0.0f);
    axesVtx[1].Color = glm::vec3(0.0f, 1.0f, 0.0f);
    axesVtx[2].Color = glm::vec3(0.0f, 0.0f, 1.0f);

    std::shared_ptr<Model> axes =
        std::make_shared<Model>(Model(std::vector<Mesh>{Mesh(axesVtx, axesInd)}));
    Renderable axesR(axes);

    player
        .scale(glm::vec3(PlayerHeight / player.getBounds().getHeight()))
        .rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        .translate(glm::vec3(-player.getBounds().min.x, -player.getBounds().min.y,
                             -player.getBounds().min.z));
    // player.BindCamera(&camera);

    quad.rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))
        .scale(glm::vec3(FovHorizontal / quadSideL));

    glm::mat4 globalProjection = camera.GetProjectionMatrice();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.renderOpts.shader = &renderer.globalShaders[Renderer::ShaderIndex::MeshDefault];
        player.renderOpts.view = camera.GetViewMatrix();
        player.renderOpts.projection = globalProjection;

        quad.renderOpts.shader = &renderer.globalShaders[Renderer::ShaderIndex::LineShader];
        quad.renderOpts.projection = camera.GetProjectionMatrice();
        quad.renderOpts.view = camera.GetViewMatrix();

        axesR.renderOpts.shader = &renderer.globalShaders[Renderer::ShaderIndex::LineShader];
        axesR.renderOpts.projection = camera.GetProjectionMatrice();
        axesR.renderOpts.view = camera.GetViewMatrix();
        axesR.renderOpts.format = GL_LINES;

        glLineWidth(20.0f); // Sets line thickness to 5 pixels
        renderer.Draw(axesR);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
        renderer.Draw(player);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
        renderer.Draw(quad);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwWindowShouldClose(window);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
