#include <parkour/Global.hpp>

GLFWwindow *initGFLFWAndGLAD(float SCR_WIDTH, float SCR_HEIGHT,

                             void (*framebuffer_size_callback)(GLFWwindow *window, int width,
                                                               int height),
                             void (*mouse_callback)(GLFWwindow *window, double xposIn,
                                                    double yposIn))
{

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "parkour", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)

{

    Global *state = (Global *)glfwGetWindowUserPointer(window);
    if (action == GLFW_PRESS)
    {
        state->keys[key].isPressed = true;
        // state->keys[mods] = true;
    }
    else
    {
        state->keys[key].isPressed = false;
        state->keys[key].wasPressed = true;
        // state->keys[mods] = false;
    }
}

void processInput(GLFWwindow *window, float deltaTime)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    Global *state = (Global *)glfwGetWindowUserPointer(window);
    auto player = state->currentSubject;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player->ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    static float lastX, lastY;
    static bool firstMouse = true;

    Global *state = (Global *)glfwGetWindowUserPointer(window);
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

    if (state->currentSubject != nullptr)
        state->currentSubject->ProcessMouseMovement(xoffset, yoffset, true);
}
