#version 330

uniform mat4 modelViewProjection;
uniform mat4 modelInverseTranspose;
uniform mat4 model;
uniform vec4 specularMaterial;
uniform vec4 specularLight;
uniform float shininess;
uniform vec3 lightDir;
uniform vec3 eyePos;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec4 vertexColour;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	vec3 transformedNormal = normalize((modelInverseTranspose * vec4(normal, 1.0)).xyz);
	vec3 transformedPosition = (model * vec4(position, 1.0)).xyz;
	vec3 toEye = normalize(eyePos - transformedPosition);
	vec3 r = reflect(-lightDir, transformedNormal);
	float t = pow(max(dot(r, toEye), 0.0), shininess);
	vertexColour = t * (specularLight * specularMaterial);
	vertexColour.a = 1.0;
}