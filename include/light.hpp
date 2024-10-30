#pragma once
#include <entity.hpp>

struct SceneLight
{
	glm::vec3 ambient;
};

class Light : public Component
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float radius;
    float intensity;

    Light() :
	    ambient(0.1f),
        diffuse(1.0f),
	    specular(0.0f),
        radius(5.0f),
        intensity(1.0f)
	{}
};