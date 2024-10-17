#pragma once
#include <glm/glm.hpp>
#include <physics.hpp>
#include <transform.hpp>

using namespace glm;

class Collider: public Component
{
public:
	// Collider transform
	vec3 offset;
	vec3 scale;
	// Constructor for Collider
	Collider() = default;
};