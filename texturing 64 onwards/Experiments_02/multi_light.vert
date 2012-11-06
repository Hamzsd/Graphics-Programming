#version 330 

uniform mat4 model;
uniform mat4 modelViewProjection;
uniform mat4 modelInverseTranspose;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 transformedPosition;
smooth out vec3 transformedNormal;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	transformedPosition = (model * vec4(position, 1.0)).xyz;
	transformedNormal = normalize((modelInverseTranspose * vec4(normal, 1.0)).xyz);
}