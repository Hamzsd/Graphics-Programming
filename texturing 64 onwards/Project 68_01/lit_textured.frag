#version 330

layout (std140) uniform;

uniform Material
{
	vec4 emissive;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

uniform Lighting
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightDir;
} lighting;

uniform sampler2D tex;

uniform vec3 eyePos;

in vec3 transformedPosition;
smooth in vec3 transformedNormal;
in vec2 texCoordOut;

out vec4 colour;

vec4 calculateLighting(in vec3 toEye)
{
	vec4 ambientColour = material.ambient * lighting.ambient;

	float s = max(dot(lighting.lightDir.xyz, transformedNormal), 0.0);
	vec4 diffuseColour = s * (material.diffuse * lighting.diffuse);

	vec3 r = reflect(-lighting.lightDir.xyz, transformedNormal);
	float t = pow(max(dot(r, toEye), 0.0), material.shininess);
	vec4 specularColour = t * (material.specular * lighting.specular);

	vec4 light = ambientColour + diffuseColour + specularColour;
	light.a = 1.0;
	return light;
}

void main()
{
	vec3 toEye = normalize(eyePos - transformedPosition);
	colour = material.emissive + (texture2D(tex, texCoordOut) * calculateLighting(toEye));
}