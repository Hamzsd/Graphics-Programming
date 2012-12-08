#version 330

uniform sampler2D tex;

const vec3 intensity = vec3(0.299, 0.587, 0.184);

in vec2 texCoordOut;

out vec4 colour;

void main()
{
	vec4 col = texture2D(tex, texCoordOut);
	colour =  1.0 - col;
	colour.a = 1.0;
}