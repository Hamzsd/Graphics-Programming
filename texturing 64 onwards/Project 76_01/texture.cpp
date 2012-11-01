#include "texture.h"
#include <IL\il.h>

texture::texture(const std::string& filename)
	: _filename(filename), _image(0)
{
}

texture::~texture()
{
	if (_image) glDeleteTextures(1, &_image);
	_image = 0;
}

bool texture::create()
{
	ILuint texid;
	ilGenImages(1, &texid);
	ilBindImage(texid);
	ILboolean success = ilLoadImage((wchar_t*)_filename.c_str());
	if (success)
	{
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			ilDeleteImages(1, &texid);
			return success;
		}
		glGenTextures(1, &_image);
		glBindTexture(GL_TEXTURE_2D, _image);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		float maxAnistropy;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnistropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnistropy);
		glTexImage2D(GL_TEXTURE_2D, 
					 0, 
					 ilGetInteger(IL_IMAGE_BPP),
					 ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT),
					 0,
					 ilGetInteger(IL_IMAGE_FORMAT),
					 GL_UNSIGNED_BYTE,
					 ilGetData());
		glGenerateMipmap(GL_TEXTURE_2D);
		ilDeleteImages(1, &texid);
		}
	return success;
}