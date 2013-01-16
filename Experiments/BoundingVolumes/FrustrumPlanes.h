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

	glm::vec4 planes[6];

private:
	void setNearPlane()
	{
		nearPlane = glm::vec4(0, 0, -1, -_cam->getNearPlane());
		nearPlane - glm::normalize(nearPlane);
		planes[0] = nearPlane;
	}

	void setFarPlane()
	{
		farPlane = glm::vec4(0, 0, 1, _cam->getFarPlane());
		farPlane - glm::normalize(farPlane);
		planes[1] = farPlane;
	}

	void setLeftPlane()
	{
		float e = getE();
		float value1 = (e / (sqrt(pow(e, 2.0f) + 1)));
		float value3 = (1 / (sqrt(pow(e, 2.0f) + 2)));
		leftPlane = glm::vec4(value1, 0 , -value3, 0);
		leftPlane - glm::normalize(leftPlane);
		planes[2] = leftPlane;
	}

	void setRightPlane()
	{
		float e = getE();
		float value1 = (e / (sqrt(pow(e, 2.0f) + 1)));
		float value3 = (1 / (sqrt(pow(e, 2.0f) + 2)));
		rightPlane = glm::vec4(-value1, 0 , -value3, 0);
		rightPlane - glm::normalize(rightPlane);
		planes[3] = rightPlane;
	}

	void setBottomPlane()
	{
		float e = getE();
		float value2 = (e / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		float value3 = (_cam->getaspectRatio() / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		bottomPlane = glm::vec4(0, value2, -value3, 0);
		bottomPlane - glm::normalize(bottomPlane);
		planes[4] = bottomPlane;
	}

	void setTopPlane()
	{
		float e = getE();
		float value2 = (e / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		float value3 = (_cam->getaspectRatio() / (sqrt(pow(e, 2.0f) + pow(_cam->getaspectRatio(), 2.0f))));
		topPlane = glm::vec4(0, -value2, -value3, 0);
		topPlane - glm::normalize(topPlane);
		planes[5] = topPlane;
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

	bool checkPointValid(glm::vec3 point, glm::mat4 transformationMatrix, camera* cam)
	{
		
		//setPlanes(cam);
		glm::vec4 _point = _cam->getView() * _cam->getProjecion() * transformationMatrix * glm::vec4(point, 1.0f);
		
		//glm::vec4 _point = _cam->getView() * _cam->getProjecion() * glm::vec4(point, 1.0f);
		
		//glm::vec4 _point = glm::vec4(point, 1.0f);
		_point = _point/_point.w;

		for (int i = 0; i < 6; ++i)
		{
			if (glm::dot(planes[i], _point) <= 0)
				return false;
			else
				return true;			
		}
	}
};