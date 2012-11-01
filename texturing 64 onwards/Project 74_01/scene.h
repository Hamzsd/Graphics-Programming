#pragma once

#include "render_object.h"
#include "light.h"

struct scene_data
{
	std::hash_map<std::string, render_object*> objects;
	std::hash_map<std::string, geometry*> geometry;
	std::hash_map<std::string, material*> material;
	std::hash_map<std::string, GLuint> textures;
	lighting light;
	dynamic_lights dynamic;
};

scene_data* loadScene(const std::string& fileName);