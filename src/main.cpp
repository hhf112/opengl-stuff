// #define DEBUG
#include "parkour/const.hpp"
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <parkour/Global.hpp>
#include <parkour/Renderable.hpp>
#include <parkour/Renderer.hpp>
#include <parkour/ResourceManager.hpp>
#include <parkour/camera.hpp>
#include <parkour/model.hpp>
#include <parkour/shader.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    Global GameState;
    Renderer &G_Rdr = GameState.renderer;
    ResourceManager &G_Inv = GameState.resources;
    Camera *G_Cam = GameState.currentCamera;
    Renderable *G_Player = GameState.currentSubject;

    GLFWwindow *window = initGFLFWAndGLAD(GameState.ScreenWidth, GameState.ScreenHeight,
                                          framebuffer_size_callback, mouse_callback);

    glfwSetWindowUserPointer(window, &GameState);
    glViewport(0, 0, GameState.ScreenWidth, GameState.ScreenHeight);




    G_Inv.loadFromJSON("./resource.json");
	// player
    Renderable player(G_Inv.models["Player"], glm::vec3(0.0f, 0.0f, 0.0f),
                      glm::vec3(1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
    auto playerBoundsMin = player.getBounds().getMin();
    player.scaleBy(glm::vec3(Game::PlayerHeight / player.getBounds().getHeight()))
        .translate(glm::vec3(0.0f, player.getHeight() / 2, 0.0f));
    // floor
    G_Inv.models["quadModel"] = Model({G_Inv.meshes["quad"]});
    Renderable quad(G_Inv.models["quadModel"], glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);
    quad.scaleBy(glm::vec3(Game::FovHorizontal));
    // axes
    G_Inv.models["axesModel"] = Model({G_Inv.meshes["axes"]});
    Renderable axesR(G_Inv.models["axesModel"], glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f);




    GameState.currentSubject = &player;
    G_Cam = &player.camera;
    G_Rdr.projection = GameState.GetProjectionMatrice();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime);

        G_Rdr.view = G_Cam->GetViewMatrix();

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        player.renderOpts.shader = &G_Inv.shaders["MeshDefault"];
        G_Rdr.Draw(player);

        quad.renderOpts.shader = &G_Inv.shaders["LineShader"];
        G_Rdr.Draw(quad);

        axesR.renderOpts.shader = &G_Inv.shaders["LineShader"];
        axesR.renderOpts.format = GL_LINES;
        glLineWidth(10.0f);
        G_Rdr.Draw(axesR);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
