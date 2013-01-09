#version 330

uniform mat4 modelViewProjection;
uniform vec4 diffuseMaterial;
uniform vec4 diffuseLight;
uniform vec3 lightDir;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
out vec4 vertexColour;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	float s = max(dot(lightDir, normal), 0.0);
	vec4 diffuseColour = s * (diffuseMaterial * diffuseLight);
	vertexColour = diffuseColour;
}