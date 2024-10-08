#include <globals.hpp>
#include <physics.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <collision.hpp>


void PhysicsUpdate(Entity& p_ent, double deltaTime)
{
	Rigidbody* rb = p_ent.GetComponent<Rigidbody>();

	// While on ground, apply friction to the velocity
	if (rb->collDown) rb->frictionVector = -rb->velocity * rb->friction;
	// Air friction
	else rb->frictionVector = -glm::vec3(rb->velocity.x, 0.0f, rb->velocity.z) * rb->friction;

	// Apply gravity
	rb->velocity.y -= Physics::gravity * static_cast<float>(deltaTime);
	// Apply friction
	rb->velocity += rb->frictionVector * static_cast<float>(deltaTime);
	// Apply velocity
	p_ent.transform.position += rb->velocity * static_cast<float>(deltaTime);
	// Reset collision flags
	rb->collDown = false, rb->collUp = false, rb->collLeft = false, rb->collRight = false, rb->collForward = false, rb->collBack = false;
}

bool CheckCollision(Entity* p_ent, Collider* b)
{
	Rigidbody* rb = p_ent->GetComponent<Rigidbody>();

	// Calculate half scales for easier collision checking
	float aHalfWidth = p_ent->transform.scale.x / 2.0f;
	float aHalfHeight = p_ent->transform.scale.y / 2.0f;
	float aHalfDepth = p_ent->transform.scale.z / 2.0f;

	float bHalfWidth = b->transform.scale.x / 2.0f;
	float bHalfHeight = b->transform.scale.y / 2.0f;
	float bHalfDepth = b->transform.scale.z / 2.0f;

	// Check for collision in the X axis
	bool collisionX = (p_ent->transform.position.x + aHalfWidth >= b->transform.position.x - bHalfWidth) &&
		(p_ent->transform.position.x - aHalfWidth <= b->transform.position.x + bHalfWidth);

	// Check for collision in the Z axis
	bool collisionZ = (p_ent->transform.position.z + aHalfDepth >= b->transform.position.z - bHalfDepth) &&
		(p_ent->transform.position.z - aHalfDepth <= b->transform.position.z + bHalfDepth);

	// Check for collision in the Y axis
	bool collisionY = (p_ent->transform.position.y + aHalfHeight >= b->transform.position.y - bHalfHeight) &&
		(p_ent->transform.position.y - aHalfHeight <= b->transform.position.y + bHalfHeight);

	// If there is a collision in all axes
	if (collisionX && collisionZ && collisionY)
	{
		// Calculate penetration depth for each axis
		float penetrationX = std::min((p_ent->transform.position.x + aHalfWidth) - (b->transform.position.x - bHalfWidth),
			(b->transform.position.x + bHalfWidth) - (p_ent->transform.position.x - aHalfWidth));

		float penetrationY = std::min((p_ent->transform.position.y + aHalfHeight) - (b->transform.position.y - bHalfHeight),
			(b->transform.position.y + bHalfHeight) - (p_ent->transform.position.y - aHalfHeight));

		float penetrationZ = std::min((p_ent->transform.position.z + aHalfDepth) - (b->transform.position.z - bHalfDepth),
			(b->transform.position.z + bHalfDepth) - (p_ent->transform.position.z - aHalfDepth));

		// Determine the axis of least penetration to resolve the collision
		if (penetrationX < penetrationY && penetrationX < penetrationZ)
		{
			// Resolve collision on the X axis
			if (p_ent->transform.position.x < b->transform.position.x) {
				p_ent->transform.position.x -= penetrationX; // Move 'a' to the left
				//rb->velocity = -rb->velocity;
				rb->collRight = true;
			}
			else {
				p_ent->transform.position.x += penetrationX; // Move 'a' to the right
				rb->collLeft = true;
			}
		}
		else if (penetrationY < penetrationX && penetrationY < penetrationZ)
		{
			// Resolve collision on the Y axis
			if (p_ent->transform.position.y < b->transform.position.y) {
				p_ent->transform.position.y -= penetrationY; // Move 'a' down
				rb->collUp = true;
				rb->velocity.y = 0.0f;
			}
			else {
				p_ent->transform.position.y += penetrationY; // Move 'a' up
				rb->collDown = true;
				if (rb->velocity.y < 0.0f) rb->velocity.y = 0.0f;
			}
		}
		else
		{
			// Resolve collision on the Z axis
			if (p_ent->transform.position.z < b->transform.position.z) {
				p_ent->transform.position.z -= penetrationZ; // Move 'a' backward
				rb->collForward = true;
			}
			else {
				p_ent->transform.position.z += penetrationZ; // Move 'a' forward
				rb->collBack = true;
			}
		}
		return true; // Collision occurred
	}

	return false; // No collision
}

namespace Physics
{
	float gravity = 9.81f;
}