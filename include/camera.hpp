#pragma once
#include <glm/glm.hpp>
#include <transform.hpp>

class Camera
{
public:
	void init();
	void move();
	Transform transform;
	float sensitivity = 0.5f;
	float fov = 90.0f;
};