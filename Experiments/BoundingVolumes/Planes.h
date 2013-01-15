#pragma once

#include <vector>
#include <glm\glm.hpp>
#include "camera.h"
#include <math.h>
#include <cmath>
#include <stdio.h>
#include "NeededHeaders.h"

struct frus
{
	camera* _cam;
	glm::vec4 nearPlane; 
	glm::vec4 farPlane;
	glm::vec4 leftPlane;
	glm::vec4 rightPlane; 
	glm::vec4 topPlane;
	glm::vec4 bottomPlane; 

	glm::mat4 modelViewProjection;

	frus() { }
	~frus() { }

	void extractPlanes(glm::mat4 model)
	{
		/*modelViewProjection = _cam->getProjecion() * _cam->getView() * model;

		nearPlane = glm::vec4(modelViewProjection[3] + modelViewProjection[2], modelViewProjection[7] + modelViewProjection[6], 
							  modelViewProjection[11] + modelViewProjection[10], modelViewProjection[15] + modelViewProjection[14]);
		nearPlane = glm::normalize(nearPlane);

		farPlane = glm::vec4(modelViewProjection[3] - modelViewProjection[2], modelViewProjection[7] - modelViewProjection[6], 
							  modelViewProjection[11] - modelViewProjection[10], modelViewProjection[15] - modelViewProjection[14]);
		farPlane = glm::normalize(farPlane);

		bottomPlane = glm::vec4(modelViewProjection[3] + modelViewProjection[1], modelViewProjection[7] + modelViewProjection[5], 
							  modelViewProjection[11] + modelViewProjection[9], modelViewProjection[15] + modelViewProjection[13]);
		bottomPlane = glm::normalize(bottomPlane);

		topPlane = glm::vec4(modelViewProjection[3] - modelViewProjection[1], modelViewProjection[7] - modelViewProjection[5], 
							  modelViewProjection[11] - modelViewProjection[9], modelViewProjection[15] - modelViewProjection[13]);
		topPlane = glm::normalize(topPlane);

		rightPlane = glm::vec4(modelViewProjection[3] - modelViewProjection[0], modelViewProjection[7] - modelViewProjection[4], 
							  modelViewProjection[11] - modelViewProjection[8], modelViewProjection[15] - modelViewProjection[12]);
		rightPlane = glm::normalize(rightPlane);

		leftPlane = glm::vec4(modelViewProjection[3] + modelViewProjection[0], modelViewProjection[7] + modelViewProjection[4], 
							  modelViewProjection[11] + modelViewProjection[8], modelViewProjection[15] + modelViewProjection[12]);
		leftPlane = glm::normalize(leftPlane);*/
	}

	//bool checkPoint(glm::vec3 point, glm::mat4 model)
	//{
		/*extractPlanes(model);

		glm::vec4 _point = glm::vec4(point, 1.0f);
		
		if (glm::dot(nearPlane, _point) < 0)
		{
			float dot = glm::dot(nearPlane, _point);
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl; 
			return false;
		}
		if (glm::dot(farPlane, _point) < 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			return false;
		}
		if (glm::dot(leftPlane, _point) < 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl; 
			return false;
		}
		if (glm::dot(rightPlane, _point) < 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			float rightAns = glm::dot(rightPlane, _point);
			return false;
		}
		if (glm::dot(topPlane, _point) < 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			return false;
		}
		if (glm::dot(bottomPlane, _point) < 0)
		{
			std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" NOT VALID"<<std::endl;
			return false;
		}
		std::cout <<"Point: " << _point.x << " , "<< _point.y << " , "<< _point.z<<" IS VALID"<<std::endl;
		return true;
*/
//	}

};