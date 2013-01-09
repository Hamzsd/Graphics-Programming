#version 330

uniform sampler2D tex;

in vec2 texCoordOut;

out vec4 colour;

void main()
{
	vec4 col = texture2D(tex, texCoordOut);
	vec4 sep = vec4(0.314, 0.169, -0.090, 1.0);
	colour = (col - sep);
	colour.a = 1.0;
}