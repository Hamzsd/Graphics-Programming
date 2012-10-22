#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

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

class first_person_camera : public camera
{
private:
	glm::quat _rotation;
	glm::vec3 _translation;
public:
	first_person_camera() : camera() { }
	~first_person_camera() { }
	glm::quat getRotation(const glm::quat& value) { _rotation = value; }
	void update(float deltaTime);
	void rotate(float deltaYaw, float deltaPitch);
	void move(const glm::vec3& translation);
};

class arc_ball_camera : public camera
{
private:
	float _rotX;
	float _rotY;
	float _minRotY;
	float _maxRotY;
	float _distance;
	float _minDistance;
	float _maxDistance;
public:
	arc_ball_camera() { }
	~arc_ball_camera() { }
	float getRotationX() const { return _rotX; }
	float setRotationX(float value) { _rotX = value; }
	float getRotationY() const { return _rotY; }
	void setRotationY(float value) { _rotY = value; }
	float getMinRotationY() const { return _minRotY; }
	void setMinRotationY(float value { _minRotY = value; }
	float getMaxRotationY() const { return _maxRotY; }
	//TODO finish arc ball camera and change render code to how it is in 56........
};