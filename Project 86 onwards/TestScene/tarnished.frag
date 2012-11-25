#version 330

uniform samplerCube cubemap;
uniform sampler2D tex;

in vec2 texCoordOut;
smooth in vec3 mapCoordOut;

out vec4 colour;

void main()
{
	vec4 col = texture(cubemap, mapCoordOut);
	col *= texture(tex, texCoordOut);
	colour = col;
}