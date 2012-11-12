#include "fireworks.h"
#include <iostream>
#include <glm\glm.hpp>

//static variable values
const float Firework::GRAVITY = 0.01f;
const float Firework::baselineYSpeed = 0.1f;
const float Firework::maxYSpeed = 0.5f;
float xs;
float ys;
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
	float ySpeedVal = baselineYSpeed + ((float)rand() / (float)RAND_MAX) * maxYSpeed;
	std::cout<< ySpeedVal << std::endl;

	//set initial x/y location and speeds for each particle in the firework
	for (int i = 0; i < FIREWORK_PARTICLES; ++i)
	{
		x[i] = xLoc;
		y[i] = -5.0f; //NOTE: in the other code this is - but i think it should be +
		z[i] = 1000.0f;
		xSpeed[i] = xSpeedVal;
		ySpeed[i] = 0.001;
		//std::cout<<"initi"<<x[0]<<" , " <<y[0]<< ", "<<zPos[0]<<std::endl;
	}
	

	//int floor = 20, ceiling = 100, range = (ceiling - floor);
	//int rnd = floor + int((range * rand()) / (RAND_MAX + 1.0));
	rnd = (rand() % 5) * 11  +  15;
	std::cout<<"\nrand " << rnd <<"\n";


	//assign random colour value
	red = ((float)rand() / (float)RAND_MAX);
	green = ((float)rand() / (float)RAND_MAX);
	blue  = ((float)rand() / (float)RAND_MAX);
	alpha = 1.0f;

	//Firework will launch after a random amount of frames between 0 and 400
	//framesUntilLaunch = ((int)rand() % 400);
	framesUntilLaunch = (5);

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
				float buffer = 3;

				float magX = glm::normalize(xSpeed[i]);
				float magY = glm::normalize(ySpeed[i]);
				float magZ = glm::normalize(zSpeed[i]);
						
				//(x-a)2 + (y-b)2 (z - c)2 = r2

				xSpeed[i]  = ((rand() / (float)RAND_MAX) * 8) ;// / magX;
				ySpeed[i] = ((rand() / (float)RAND_MAX) * 1) ;// / magY;
				zSpeed[i]  = ((rand() / (float)RAND_MAX) * 1);// / magZ;
											


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
		// Dampen the horizontal speed by 1% per frame
		//xSpeed[loop] *= 0.99;
 
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

	
	