#include <player.hpp>
#include <globals.hpp>
#include <inputs.hpp>
#include <iostream>

const float walkSpeed = 100.0f;
const float sprintSpeed = 150.0f;
float jumpStrength = 10.0f;
float playerSpeed;

float airMultiplier = 0.75f;

enum class PlayerState
{
	AIR,
	WALKING,
	SPRINTING
};

PlayerState playerState = PlayerState::AIR;
Rigidbody* rb;

void Player::init()
{
	rb = entity.GetComponent<Rigidbody>();
	entity.transform.position = glm::vec3(-3.0f, 10.0f, 0.0f);
	entity.transform.forward = glm::vec3(0.0f, 0.0f, -1.0f);
	entity.transform.scale = glm::vec3(1.0f, 3.5f, 1.0f);
	entity.transform.up = glm::vec3(0.0f, 1.0f, 0.0f);
	//player.rb.transform = &player.transform;
}

void Player::move()
{
	if (rb == nullptr)
	{
		std::cerr << "Rigidbody not found on player entity!" << std::endl;
		return;
	}
	if (!rb->collDown) playerState = PlayerState::AIR;
	else playerState = Input::GetKeyDown(KeyCode::SHIFT) ? PlayerState::SPRINTING : PlayerState::WALKING;

	//playerSpeed = (playerState == PlayerState::AIR ? () : ()) * Time::deltaTime;
	if (playerState == PlayerState::AIR)
	{
		playerSpeed = walkSpeed * airMultiplier * Time::deltaTime;
	}
	else
	{
		playerSpeed = (playerState == PlayerState::SPRINTING ? sprintSpeed : walkSpeed) * Time::deltaTime;
	}
	

	entity.transform.rotation = glm::vec3(cam.transform.rotation.x, entity.transform.rotation.y, cam.transform.rotation.z);

	glm::vec3 front;
	front.x = cos(glm::radians(entity.transform.rotation.x)) * cos(glm::radians(entity.transform.rotation.y));
	front.y = sin(glm::radians(entity.transform.rotation.y));
	front.z = sin(glm::radians(entity.transform.rotation.x)) * cos(glm::radians(entity.transform.rotation.y));
	entity.transform.forward = normalize(front);

	glm::vec3 moveDir = glm::vec3(0.0f);
	moveDir.y += Input::GetKeyDown(KeyCode::W) ? 1 : 0;
	moveDir.x += Input::GetKeyDown(KeyCode::A) ? -1 : 0;
	moveDir.y += Input::GetKeyDown(KeyCode::S) ? -1 : 0;
	moveDir.x += Input::GetKeyDown(KeyCode::D) ? 1 : 0;

	if (glm::length(moveDir) > 0.0f)
	{
		moveDir = glm::normalize(moveDir);
	}

	rb->velocity += playerSpeed * ((entity.transform.forward * moveDir.y) + glm::normalize(glm::cross(entity.transform.forward, entity.transform.up)) * moveDir.x);

	if (Input::GetKeyDown(KeyCode::SPACE) && rb->collDown)
	{
		rb->velocity.y =  1.0f * jumpStrength;
	}
}