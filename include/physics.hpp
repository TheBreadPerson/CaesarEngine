#pragma once
#include <transform.hpp>
#include <glm/glm.hpp>
#include <collision.hpp>

class Collider;

class Rigidbody : public Component
{
public:
	glm::vec3 velocity;

	// PHYSICS OBJECT SETTINGS
	float friction = 10.0f;
	glm::vec3 frictionVector;


	bool collDown = false;  // Colliding on the downward (Y-axis negative)
	bool collUp = false;    // Colliding on the upward (Y-axis positive)
	bool collLeft = false;  // Colliding on the left (X-axis negative)
	bool collRight = false; // Colliding on the right (X-axis positive)
	bool collForward = false; // Colliding forward (Z-axis positive)
	bool collBack = false;    // Colliding backward (Z-axis negative)

	Rigidbody() : velocity(0.0f) {};
};

void PhysicsUpdate(Entity& p_ent, double deltaTime);

bool CheckCollision(Entity* p_ent, Collider* b);

namespace Physics
{
	extern float gravity;
}