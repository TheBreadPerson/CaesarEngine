#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class KeyCode
{
	W = GLFW_KEY_W,
	A = GLFW_KEY_A,
	S = GLFW_KEY_S,
	D = GLFW_KEY_D,
	R = GLFW_KEY_R,
	P = GLFW_KEY_P,
	SPACE = GLFW_KEY_SPACE,
	SHIFT = GLFW_KEY_LEFT_SHIFT,
	UP = GLFW_KEY_UP,
	DOWN = GLFW_KEY_DOWN,
	ESC = GLFW_KEY_ESCAPE
};
namespace Input
{
	void processInput();
	bool GetKeyDown(KeyCode key);
	bool GetKeyUp(KeyCode key);
	

	void GetMouse(float& x, float& y);
	void GetMouseDelta(float& x, float& y);
};