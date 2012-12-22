#include "cubemap.h"


cubemap::cubemap(const std::vector<std::string>& filenames)
	: _filenames(filenames), _image(0)
{
}

cubemap::~cubemap()
{
	if (_image) glDeleteTextures(1, &_image);
	_image = 0;
}

bool cubemap::create()
{
	// first use devil to load in the image data
	ILuint texid[6];

	//create a devil image
	ilGenImages(6, texid);

	//try and load each image for the cube map
	for (int i = 0; i < 6; ++i)
	{
		// bind image
		ilBindImage(texid[i]);

		//load in the image into memor
		ILboolean success = ilLoadImage((wchar_t*)_filenames[i].c_str());
		if (!success)
			return success;

		//convert the image into RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			//delete all created images
			for (int j = 0; j <= i; ++j)
				ilDeleteImages(1, &texid[j]);
			return success;
		}
	}

	glActiveTexture(GL_TEXTURE0);

	// Enable cube map state
	glEnable(GL_TEXTURE_CUBE_MAP);
	CHECK_GL_ERROR

	//now use OPENGL to create image buffer
	glGenTextures(1, &_image);
	CHECK_GL_ERROR

	//Bind texture as cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, _image);
	CHECK_GL_ERROR

	//set up texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR

	// create the cube map image by assigning each texture
	GLenum targets[6] = 
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; ++i)
	{
		ilBindImage(texid[i]);
		glTexImage2D(targets[i],
					 0, 
					 ilGetInteger(IL_IMAGE_BPP),
					 ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT),
					 0,
					 ilGetInteger(IL_IMAGE_FORMAT),
					 GL_UNSIGNED_BYTE,
					 ilGetData());
		CHECK_GL_ERROR
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//delete the devil images
	ilDeleteImages(6, texid);

	return true;
}