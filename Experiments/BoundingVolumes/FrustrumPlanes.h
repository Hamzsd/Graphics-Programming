#pragma once

#include <vector>
#include <glm\glm.hpp>
#include "camera.h"
#include <math.h>
#include <cmath>

struct frustrumPlanes
{	
	glm::vec4 nearPlane;
	glm::vec4 farPlane;
	glm::vec4 rightPlane;
	glm::vec4 leftPlane;
	glm::vec4 topPlane;
	glm::vec4 bottomPlane;
	camera* _cam; 

private:
	void setNearPlane()
	{
		nearPlane = glm::vec4(0, 0, -1, -_cam->getNearPlane());
	}

	void setFarPlane()
	{
		farPlane = glm::vec4(0, 0, 1, _cam->getFarPlane());
	}

	void setLeftPlane()
	{
		float e = getE();
		float value1 = (e / (sqrt(pow(e, 2.0f) + 1)));
		float value3 = (1 / (sqrt(pow(e, 2.0f) + 2)));
		leftPlane = glm::vec4(value1, 0 , -value3, 0);
	}

	void setRightPlane()
	{
		float e = getE();
		float value1 = (e / (sqrt(pow(e, 2.0f) + 1)));
		float value3 = (1 / (sqrt(pow(e, 2.0f) + 2)));
		rightPlane = glm::vec4(-value1, 0 , -value3, 0);
	}

	void setBottomPlane()
	{
		float e = getE();
		float value2 = (e / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		float value3 = (_cam->getaspectRatio() / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		bottomPlane = glm::vec4(0, value2, -value3, 0);
	}

	void setTopPlane()
	{
		float e = getE();
		float value2 = (e / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		float value3 = (_cam->getaspectRatio() / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		topPlane = glm::vec4(0, -value2, -value3, 0);
	}

	float getE()
	{
		float e; 
		return e = 1 / tan((_cam->getFov()/ 2));
	}

public:
	
	frustrumPlanes() { }
	~frustrumPlanes(){ }

	void setPlanes(camera* cam)
	{
		_cam = cam;
		setNearPlane();			setFarPlane();
		setLeftPlane();			setRightPlane();
		setBottomPlane();		setTopPlane();
	}

	glm::vec4 getNearPlane() { return nearPlane; } 
	glm::vec4 getFarPlane() { return farPlane; }
	glm::vec4 getLeftPlane() { return leftPlane; } 
	glm::vec4 getRightPlane() { return rightPlane; } 
	glm::vec4 getTopPlane() { return topPlane; }
	glm::vec4 getBottomPlane() { return bottomPlane; } 	

	bool checkPointValid(glm::vec3 point, glm::mat4 transformationMatrix)
	{
		//glm::vec4 _point = _cam->getView() * _cam->getProjecion() * transformationMatrix * glm::vec4(point, 1.0f);
		glm::vec4 _point = _cam->getView() * _cam->getProjecion() * glm::vec4(point, 1.0f);
		//_point = glm::vec4((_point.x / _point.w, _point.y / _point.w, _point.z / _point.w, _point.w / _point.w));

		//glm::vec4 _point = glm::vec4(point, 1.0f);

		if (glm::dot(nearPlane, _point) <= 0)
		{
			float dot = glm::dot(nearPlane, _point);
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl; 
			return false;
		}
		if (glm::dot(farPlane, _point) <= 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			return false;
		}
		if (glm::dot(leftPlane, _point) <= 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl; 
			return false;
		}
		if (glm::dot(rightPlane, _point) <= 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			float rightAns = glm::dot(rightPlane, _point);
			return false;
		}
		if (glm::dot(topPlane, _point) <= 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			return false;
		}
		if (glm::dot(bottomPlane, _point) <= 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			return false;
		}
		std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" IS VALID"<<std::endl;
		return true;
	}
};