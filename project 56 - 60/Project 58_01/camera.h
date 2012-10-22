#pragma once

#include <glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>

class camera
{
protected:
	glm::vec3 _position;
	glm::vec3 _target;
	glm::vec3 _up;
	glm::mat4 _view;
	glm::mat4 _projection;
public:
	camera() { }
	virtual ~camera() { }
	virtual void update(float deltaTime) = 0;
	glm::vec3 getPosition() const { return _position; }
	void setPositon(const glm::vec3& value) { _position = value; }
	glm::vec3 getTarget() const { return _target; }
	void setTarget(const glm::vec3& value) { _target = value; }
	glm::vec3 getUp() const { return _up; }
	void setUp(const glm::vec3& value) { _up = value; }
	glm::mat4 getView() const { return _view; } 
	glm::mat4 getProjecion() const { return _projection; }
	void setProjection(float fov, float aspect, float nearPlane, float farPlane)
	{
		_projection = glm::perspective(glm::degrees(fov), aspect, nearPlane, farPlane);
	}
};

class target_camera : public camera
{
public: 
	target_camera() : camera() { }
	~target_camera() { }
	void update(float deltaTime);
};