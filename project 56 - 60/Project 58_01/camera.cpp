#include "camera.h"

void target_camera::update(float deltaTime)
{
	glm::vec3 forward = _target - _position;
	glm::vec3 side = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
	_up = glm::cross(forward, side);
	_up = glm::normalize(_up);
	_view = glm::lookAt(_position, _target, _up);
}