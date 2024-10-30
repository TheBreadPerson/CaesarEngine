#include <player.hpp>
#include <globals.hpp>
#include <inputs.hpp>
#include <iostream>

const float walkSpeed = 50.0f;
const float sprintSpeed = 75.0f;
float jumpStrength = 4.5f;
float playerSpeed;

float airMultiplier = 0.75f;

bool noclip = false;

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
	else playerState = Input::GetKey(KeyCode::SHIFT) ? PlayerState::SPRINTING : PlayerState::WALKING;

	// ANT MAN SCALING
	if (Input::GetKey(KeyCode::UP))
	{
		entity.GetComponent<Collider>()->scale.y += 5.0f * Time::deltaTime;
	}
	if (Input::GetKey(KeyCode::DOWN))
	{
		entity.GetComponent<Rigidbody>()->velocity.y -= 9.81f * Time::deltaTime;
		if (entity.GetComponent<Collider>()->scale.y >= 0.00000001f) entity.GetComponent<Collider>()->scale.y -= 4.0f * Time::deltaTime;
		//entity.transform.scale.y -= 0.1f;
	}

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
	moveDir.y += Input::GetKey(KeyCode::W) ? 1 : 0;
	moveDir.x += Input::GetKey(KeyCode::A) ? -1 : 0;
	moveDir.y += Input::GetKey(KeyCode::S) ? -1 : 0;
	moveDir.x += Input::GetKey(KeyCode::D) ? 1 : 0;

	if (glm::length(moveDir) > 0.0f)
	{
		moveDir = glm::normalize(moveDir);
	}

	if (!noclip)
	{
		entity.GetComponent<Collider>()->enabled = true;
		entity.GetComponent<Rigidbody>()->enabled = true;
		rb->velocity += playerSpeed * ((entity.transform.forward * moveDir.y) + glm::normalize(glm::cross(entity.transform.forward, entity.transform.up)) * moveDir.x);
	}
	// NOCLIP CONTROLLER
	if (noclip)
	{
		entity.GetComponent<Collider>()->enabled = false;
		entity.GetComponent<Rigidbody>()->enabled = false;
		entity.transform.position += playerSpeed * ((cam.transform.forward * moveDir.y) + glm::normalize(glm::cross(cam.transform.forward, entity.transform.up)) * moveDir.x);
		if (Input::GetKey(KeyCode::SPACE))
		{
			entity.transform.position.y += playerSpeed/2.0f;
		}
		if (Input::GetKey(KeyCode::LCTRL))
		{
			entity.transform.position.y -= playerSpeed/2.0f;
		}
	}
	

	if (Input::GetKey(KeyCode::SPACE) && rb->collDown)
	{
		rb->velocity.y = 1.0f * jumpStrength;
	}

	if (Input::GetKeyDown(KeyCode::N))
	{
		noclip = !noclip;
	}
}