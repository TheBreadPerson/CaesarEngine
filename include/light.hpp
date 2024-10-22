#pragma once
#include <entity.hpp>

class Light : public Component
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float intensity;

    Light() :
	    ambient(0.1f),
        diffuse(1.0f),
	    specular(0.0f),
		constant(1.0f),
		linear(0.09f),
		quadratic(0.032f),
		intensity(1.0f)
	{}
};