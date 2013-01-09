#version 330

uniform mat4 modelViewProjection;
uniform vec4 ambientMaterial;
uniform vec4 ambientLight;

layout (location = 0) in vec3 position;
out vec4 vertexColour;

void main()
{
	gl_Position = modelViewProjection * vec4(position, 1.0);
	vertexColour = ambientMaterial * ambientLight;
}