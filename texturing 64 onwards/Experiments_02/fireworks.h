#pragma once

#include <cstdlib>
#include <GL\glfw.h>

const GLint FIREWORK_PARTICLES = 100;

class Firework
{
	public:
		float x[FIREWORK_PARTICLES];
		float y[FIREWORK_PARTICLES];
		float zPos[FIREWORK_PARTICLES];
		float xSpeed[FIREWORK_PARTICLES];
		float ySpeed[FIREWORK_PARTICLES];
 
		float red;
		float blue;
		float green;
		float alpha;
 
		int framesUntilLaunch;
 
		float particleSize;
		bool hasExploded;
 
		static const float baselineYSpeed;
		static const float maxYSpeed;
		static const float GRAVITY;
 
		Firework(); 
		void initialise();
		void move();
		void explode();
};