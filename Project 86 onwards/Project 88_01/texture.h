#pragma once

#include <string>
#include <glew.h>

class texture
{
private:
	GLuint _image;
	std::string _filename;
public:
	texture(const std::string& filename);
	~texture();
	GLuint getImageID() const { return _image; }
	bool create();
};