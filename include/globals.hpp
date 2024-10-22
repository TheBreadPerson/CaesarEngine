#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <renderer.hpp>
#include <player.hpp>
#include <camera.hpp>
#include <editor.hpp>




extern GLFWwindow* window;
extern Camera cam;
extern Player player;
extern Renderer renderer;
extern Editor editor;

extern unsigned int screen_width;
extern unsigned int screen_height;

extern unsigned int defaultShaderID;


static float reMap(float x, float iMin, float iMax, float oMin, float oMax);

namespace Time
{
	extern double deltaTime;
	void Tick();
}