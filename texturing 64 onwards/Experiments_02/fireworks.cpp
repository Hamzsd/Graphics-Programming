#include "fireworks.h"
#include <iostream>

//static variable values
const float Firework::GRAVITY = 0.05f;
const float Firework::baselineYSpeed = 0.1f;
const float Firework::maxYSpeed = 0.5f;
float xs;
float ys;

Firework::Firework()
{
	//call initialise so that object doesnt need to be destroyed when it is re-initialised
	initialise();
}

void Firework::initialise()
{
	float xLoc = ((float)rand() / (float)RAND_MAX) * 800.0f;
	float xSpeedVal = 2 + ((float)rand() / (float)RAND_MAX) * 4.0f;
	float ySpeedVal = baselineYSpeed + ((float)rand() / (float)RAND_MAX) * maxYSpeed;
	std::cout<< ySpeedVal << std::endl;

	//set initial x/y location and speeds for each particle in the firework
	for (int i = 0; i < FIREWORK_PARTICLES; ++i)
	{
		x[i] = 0.0f;
		y[i] = -100.0f; //NOTE: in the other code this is - but i think it should be +
		zPos[i] = -0.0f;
		xSpeed[i] = 0.001;
		ySpeed[i] = 0.001;
		//std::cout<<"initi"<<x[0]<<" , " <<y[0]<< ", "<<zPos[0]<<std::endl;
	}
	

	//assign random colour value
	red = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue  = ((float)rand() / (float)RAND_MAX);
	alpha = 1.0f;

	//Firework will launch after a random amount of frames between 0 and 400
	framesUntilLaunch = ((int)rand() % 400);

	//size of the particle (as thrown to glPointSize) - rand is 1.0f to 4.0f
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;

	// Flag to keep track of wherter the firework has exploaded
	hasExploded = false;

	xs = 0.1;
	ys = 0.1;

	std::cout<< "initialised a firework." <<std::endl;
}

void Firework::move()
{
	for (int i = 0; i < FIREWORK_PARTICLES; ++i)
	{
		//once ready to launch start moving particles
		if (framesUntilLaunch <= 0)
		{
			//x[i] += xSpeed[i];
			y[i] += ySpeed[i];
			ySpeed[i] += Firework::GRAVITY;
		}
	}
		framesUntilLaunch--; //decrease countdown

	

	//once pos turns positive - make it go boom
		if (y[0] > 4.0f)
		{
			for (int i = 0; i < FIREWORK_PARTICLES; ++i)
			{
				xSpeed[i] = -4 + (rand() / (float)RAND_MAX) * 8;
				ySpeed[i] = -4 + (rand() / (float)RAND_MAX) * 8;
				/*x[i] -= xs;
				y[i] += ys;*/
			}
			hasExploded = true;
			std::cout<<"booom " <<x[0]<<" , "<<y[0]<<" , "<<zPos[0]<<std::endl;
		}
}

void Firework::explode()
{

	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{
		// Dampen the horizontal speed by 1% per frame
		xSpeed[loop] *= 0.99;
 
		// Move the particle
		x[loop] += xSpeed[loop];
		y[loop] += ySpeed[loop];
 
		// Apply gravity to the particle's speed
		ySpeed[loop] += Firework::GRAVITY;
	}
	
	/*for (int i = 0; i < FIREWORK_PARTICLES; ++i)
	{
		
		if (i < 25)
		{
			x[i] -= xs;
			y[i] += ys;
		}
		if (i > 25 && i < 50)
		{
			x[i] -= xs;
			y[i] -= ys;
		}
		if (i > 50 && i < 75)
		{
			x[i] += xs;
			y[i] -= ys;
		}
		if (i > 75 && i <100)
		{
			x[i] += xs;
			y[i] += ys;
		}

		
	}
	std::cout<<"pos at explosition " <<x[0]<<" , "<<y[0]<<" , "<<zPos[0]<<std::endl;*/

	
	//fade out particles
	if (alpha > 0.0f)
		alpha -= 0.01f;
	else
		initialise();
}