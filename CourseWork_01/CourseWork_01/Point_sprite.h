#pragma once

#include "geometry.h"
#include "Transform.h"
#include "effect.h"
#include "texture.h"
#include "camera.h"

/*
a class used to store a collection of points 
that can be rendered as point sprites
*/
class point_sprite
{
private:
	// Geometry struct used to hold point data
	geometry* _geom;

	// Texture struct used for point sprite
	texture* _tex;
public:
	/* Transfor for the point sprite object. might be needed for future 
	implementations 
	*/
	transform transform;

	//Creates the point sprite object with the given geometry and texture
	point_sprite(geometry* geom, texture* tex);

	//Destructor
	~point_sprite();

	// Initialies values required to use point sprites

	bool create();

	// Renders the point sprites to the screen

	void render(effect* eff, const camera* camera);
};

// Helper function to create point data that can act as a star field
geometry* createStarField(int num_stars);

