#version 330

uniform sampler2D tex;

uniform vec3 eyePos;

in vec3 transformedPosition;
smooth in vec3 transformedNormal;
in vec2 texCoordOut;

out vec4 colour;

vec4 calculateLighting(in vec3 toEye, in vec3 transformedNormal);

void main()
{
	vec3 toEye = normalize(eyePos - transformedPosition);
	colour = (texture2D(tex, texCoordOut) * calculateLighting(toEye, transformedNormal));
}