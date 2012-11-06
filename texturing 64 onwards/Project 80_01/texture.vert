#version 330

uniform mat4 modelViewProjection;

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoordIn;

out vec2 texCoordOut;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	texCoordOut = texCoordIn;
}