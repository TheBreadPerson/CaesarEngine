#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <memory>
#include "renderer.hpp"
#include "inputs.hpp"
#include "camera.hpp"
#include "globals.hpp"
#include "scene.hpp"
#include "gamescene.hpp"

GLFWwindow* window;
Renderer renderer;
Camera cam;
Player player;

SceneManager sceneManager;
std::shared_ptr<GameScene> gameScene = std::make_shared<GameScene>();

unsigned int screen_width = 800;
unsigned int screen_height = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    screen_width = width;
    screen_height = height;
}

void init()
{
    sceneManager.openScene(gameScene);
    cam.init();
    player.init();
    // GLFW Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Error handling
    window = glfwCreateWindow(screen_width, screen_height, "Caesar Engine", NULL, NULL);
    if (window == nullptr)
    {
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    renderer.init();
}

void GameLoop()
{
	gameScene->Update();
}

void FixedGameLoop(double deltaTime)
{
    gameScene->FixedUpdate(deltaTime);
}

int main()
{
    init();
    while (!glfwWindowShouldClose(window))
    {
        Time::Tick();
        if (Input::GetKeyDown(KeyCode::ESC))
        {
            glfwSetWindowShouldClose(window, true);
        }

        GameLoop();
        FixedGameLoop(Time::deltaTime);

        // input
        Input::processInput();

        // rendering
        renderer.draw(window);
    }
    glfwTerminate();
    return 0;
}