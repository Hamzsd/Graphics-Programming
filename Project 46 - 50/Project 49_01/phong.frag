#version 330

uniform vec4 ambientLight;
uniform vec4 diffuseLight;
uniform vec4 specularLight;

uniform vec4 ambientMaterial;
uniform vec4 diffuseMaterial;
uniform vec4 specularMaterial;
uniform float shininess;

uniform vec3 lightDir;
uniform vec3 eyePos;

in vec3 transformedPosition;
smooth in vec3 transformedNormal;

out vec4 colour;

void main()
{
	//ambient light
	vec4 ambientColour = ambientMaterial * ambientLight;

	//DiffuseLight
	float s = max(dot(lightDir, transformedNormal), 0.0);
	vec4 diffuseColour = s * (diffuseMaterial * diffuseLight);

	// Specular light
	vec3 toEye = normalize(eyePos - transformedPosition);
	vec3 r = reflect(-lightDir, transformedNormal);
	float t = pow(max(dot(r, toEye), 0.0), shininess);
	vec4 specularColour = t * (specularLight * specularMaterial);

	colour = ambientColour + diffuseColour + specularColour;
	colour.a = 1.0;
}