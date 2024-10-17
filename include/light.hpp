#pragma once
#include <entity.hpp>

class Light : public Component
{
public:
	glm::vec3 color;
	float intensity;

    Light() :
    color(1.0f),
	intensity(1.0f)
	{}
};