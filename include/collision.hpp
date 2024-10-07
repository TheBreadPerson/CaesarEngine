#pragma once
#include <glm/glm.hpp>
#include <physics.hpp>
#include <transform.hpp>

using namespace glm;

class Collider: public Component
{
public:
	// Collider transform
	Transform transform;
	// Constructor for Collider
	Collider() = default;
};