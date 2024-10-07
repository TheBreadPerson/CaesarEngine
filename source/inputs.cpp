#include "globals.hpp"
#include "camera.hpp"
#include "inputs.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

bool firstMouse;
//glm::vec2 moveAxis = glm::vec2();
float lastX, lastY;
float xpos;
float ypos;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	xpos = static_cast<float>(xposIn);
	ypos = static_cast<float>(yposIn);
}

void Input::processInput()
{
	glfwPollEvents();
	glfwSetCursorPosCallback(window, mouse_callback);
}

bool Input::GetKeyDown(KeyCode key)
{
	return glfwGetKey(window, static_cast<int>(key)) == GLFW_PRESS;
}
bool Input::GetKeyUp(KeyCode key)
{
	return glfwGetKey(window, static_cast<int>(key)) == GLFW_RELEASE;
}

void Input::GetMouse(float& x, float& y)
{
	x = xpos;
	y = ypos;
}

void Input::GetMouseDelta(float& x, float& y)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	x = xoffset;
	y = yoffset;
}