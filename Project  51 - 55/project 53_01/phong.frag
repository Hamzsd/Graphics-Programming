#version 330

layout (std140) uniform;

uniform Light
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightDir;
} light;

uniform Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

uniform vec3 eyePos;

in vec3 transformedPosition;
smooth in vec3 transformedNormal;

out vec4 colour;

void main()
{
	//ambient light
	vec4 ambientColour = material.ambient * light.ambient;

	//DiffuseLight
	float s = max(dot(light.lightDir.xyz, transformedNormal), 0.0);
	vec4 diffuseColour = s * (material.diffuse * light.diffuse);

	// Specular light
	vec3 toEye = normalize(eyePos - transformedPosition);
	vec3 r = reflect(-light.lightDir.xyz, transformedNormal);
	float t = pow(max(dot(r, toEye), 0.0), material.shininess);
	vec4 specularColour = t * (material.specular * light.specular);

	colour = ambientColour + diffuseColour + specularColour;
	colour.a = 1.0;
}