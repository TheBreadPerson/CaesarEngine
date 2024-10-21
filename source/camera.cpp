#include <globals.hpp>
#include <camera.hpp>
#include <inputs.hpp>
#include <glm/glm.hpp>


float mx, my;
float deltaMX, deltaMY;
void Camera::init()
{
	Camera::transform.position = glm::vec3(0.0f);
	Camera::transform.forward = glm::vec3(0.0f, 0.0f, -1.0f);
	Camera::transform.up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::move()
{
	float mX, mY;
	Input::GetMouse(mX, mY);
	float deltaMX, deltaMY;
	Input::GetMouseDelta(deltaMX, deltaMY);
	Camera::transform.rotation.x += (deltaMX)*sensitivity;
	Camera::transform.rotation.y += (deltaMY)*sensitivity;
	if (Camera::transform.rotation.y > 89.0f)
		Camera::transform.rotation.y = 89.0f;
	if (Camera::transform.rotation.y < -89.0f)
		Camera::transform.rotation.y = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Camera::transform.rotation.x)) * cos(glm::radians(Camera::transform.rotation.y));
	front.y = sin(glm::radians(Camera::transform.rotation.y));
	front.z = sin(glm::radians(Camera::transform.rotation.x)) * cos(glm::radians(Camera::transform.rotation.y));
	Camera::transform.forward = normalize(front);

	Camera::transform.position = player.entity.transform.position;
}