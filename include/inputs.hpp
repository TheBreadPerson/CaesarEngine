#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum class KeyCode
{
	A = GLFW_KEY_A,
	B = GLFW_KEY_B,
	C = GLFW_KEY_C,
	D = GLFW_KEY_D,
	E = GLFW_KEY_E,
	F = GLFW_KEY_F,
	G = GLFW_KEY_G,
	H = GLFW_KEY_H,
	I = GLFW_KEY_I,
	J = GLFW_KEY_J,
	K = GLFW_KEY_K,
	L = GLFW_KEY_L,
	M = GLFW_KEY_M,
	N = GLFW_KEY_N,
	O = GLFW_KEY_O,
	P = GLFW_KEY_P,
	Q = GLFW_KEY_Q,
	R = GLFW_KEY_R,
	S = GLFW_KEY_S,
	T = GLFW_KEY_T,
	U = GLFW_KEY_U,
	V = GLFW_KEY_V,
	W = GLFW_KEY_W,
	X = GLFW_KEY_X,
	Y = GLFW_KEY_Y,
	Z = GLFW_KEY_Z,

	SPACE = GLFW_KEY_SPACE,
	LCTRL = GLFW_KEY_LEFT_CONTROL,
	SHIFT = GLFW_KEY_LEFT_SHIFT,
	UP = GLFW_KEY_UP,
	DOWN = GLFW_KEY_DOWN,
	RIGHT = GLFW_KEY_RIGHT,
	LEFT = GLFW_KEY_LEFT,
	ESC = GLFW_KEY_ESCAPE
};
namespace Input
{
	void processInput();
	bool GetKeyDown(KeyCode key);
	bool GetKeyUp(KeyCode key);
	bool GetKey(KeyCode key);

	void GetMouse(float& x, float& y);
	void GetMouseDelta(float& x, float& y);
};