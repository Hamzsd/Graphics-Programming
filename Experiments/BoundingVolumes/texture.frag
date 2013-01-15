#version 330

layout (std140) uniform;

uniform sampler2D tex;

uniform Material
{
	vec4 emissive;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

in vec2 texCoordOut;

out vec4 colour;

void main()
{
	colour = texture2D(tex, texCoordOut);
}