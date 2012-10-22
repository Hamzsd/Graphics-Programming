#include "camera.h"

void target_camera::update(float deltaTime)
{
	glm::vec3 forward = _target - _position;
	glm::vec3 side = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), forward);
	_up = glm::cross(forward, side);
	_up = glm::normalize(_up);
	_view = glm::lookAt(_position, _target, _up);
}

void first_person_camera::update(float deltaTime)
{
	glm::mat4 mat = glm::mat4_cast(_rotation);
	glm::vec4 vec(_translation, 1.0f);
	vec = mat * vec;
	_translation = glm::vec3(vec.x, vec.y, vec.z);
	_position += _translation;
	_translation = glm::vec3 (0.0f, 0.0f, 0.0f);

	glm::vec4 forward(0.0f, 0.0f, 1.0f, 1.0f);
	forward = mat * forward;
	_target = _position + glm::vec3(forward.x, forward.y, forward.z);

	glm::vec4 tempUp(0.0f, 1.0f, .0f, 1.0f);
	tempUp = mat * tempUp;
	_up = glm::vec3(tempUp.x, tempUp.y, tempUp.z);

	_view = glm::lookAt(_position, _target, _up);
}

void first_person_camera::rotate(float deltaYaw, float deltaPitch)
{
	_rotation = glm::rotate(_rotation, glm::degrees(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));
	_rotation = glm::rotate(_rotation, glm::degrees(deltaPitch), glm::vec3(1.0f, 0.0f, 0.0));
	_rotation = glm::normalize(_rotation);
}

void first_person_camera::move(const glm::vec3& translation)
{
	_translation += translation;
}