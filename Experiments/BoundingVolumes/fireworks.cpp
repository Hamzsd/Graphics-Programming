#include "fireworks.h"
#include <iostream>
#include <glm\glm.hpp>

//static variable values
const float Firework::GRAVITY = 0.01f;

int rnd = 0;



Firework::Firework()
{
	//call initialise so that object doesnt need to be destroyed when it is re-initialised
	initialise();
}

void Firework::initialise()
{
	float xLoc = ((float)rand() / (float)RAND_MAX) * 800.0f;
	float xSpeedVal = 2 + ((float)rand() / (float)RAND_MAX) * 4.0f;
	if(rand()%2)
	{
		xSpeedVal = -xSpeedVal;
		xLoc = -xLoc;
	}
	if(rand()%2)
	{
		xSpeedVal = 0.0f;
		xLoc = 0.0f;
	}

	//set initial x/y location and speeds for each particle in the firework
	for (int i = 0; i < FIREWORK_PARTICLES; ++i)
	{
		x[i] = xLoc;
		y[i] = -5.0f; 
		z[i] = 1000.0f;
		xSpeed[i] = xSpeedVal;
		ySpeed[i] = 0.001;
	}

	rnd = (rand() % 5) * 11  +  15;
	std::cout<<"\nrand " << rnd <<"\n";

	//assign random colour value
	red = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue  = ((float)rand() / (float)RAND_MAX);
	alpha = 1.0f;

	framesUntilLaunch = (5);

	//size of the particle (as thrown to glPointSize) - rand is 1.0f to 4.0f
	particleSize = 1.0f + ((float)rand() / (float)RAND_MAX) * 3.0f;

	// Flag to keep track of wherter the firework has exploaded
	hasExploded = false;

	std::cout<< "initialised a firework." <<std::endl;
}

void Firework::move()
{
	
	for (int i = 0; i < FIREWORK_PARTICLES; ++i)
	{
		
		//once ready to launch start moving particles
		if (framesUntilLaunch <= 0)
		{
			x[i] += xSpeed[i];
			y[i] += ySpeed[i];
			ySpeed[i] += Firework::GRAVITY;	
		}
	}
		framesUntilLaunch--; //decrease countdown

	//once pos turns positive - make it go boom
		if (y[0] > (rnd))
		{
			for (int i = 0; i < FIREWORK_PARTICLES; ++i)
			{
										
				xSpeed[i]  = ((rand() / (float)RAND_MAX) * 8);
				ySpeed[i] = ((rand() / (float)RAND_MAX) * 1);
				zSpeed[i]  = ((rand() / (float)RAND_MAX) * 1);
											
				if(rand()%2)
				{
					xSpeed[i] = -xSpeed[i];
				}

				if(rand()%2)
				{
					ySpeed[i] = -ySpeed[i];
				}

				if(rand()%2)
				{
					zSpeed[i] = -zSpeed[i];
				}
			}
			hasExploded = true;
			std::cout<<"booom " <<x[0]<<" , "<<y[0]<<" , "<<z[0]<<std::endl;
			
		}
}

void Firework::explode()
{

	for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
	{ 
		// Move the particle
		x[loop] += xSpeed[loop];
		y[loop] += ySpeed[loop];
		z[loop] += zSpeed[loop];
 
		// Apply gravity to the particle's speed
		ySpeed[loop] += -Firework::GRAVITY;
	}

	//fade out particles
	if (alpha > 0.0f)
	{
		alpha -= 0.01f;
		for (int loop = 0; loop < FIREWORK_PARTICLES; loop++)
		{
			xSpeed[loop] -= 0.001f;
			ySpeed[loop] -= 0.001f;
			zSpeed[loop] -= 0.001f;
		}
	}
	else
		initialise();
}
	
	

	
	