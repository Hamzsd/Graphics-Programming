#version 330

layout(std140) uniform;

uniform Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
} material;

uniform PointLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightPos;
	vec4 attenuation;
	float dist;
} point;

uniform vec3 eyePos;

in vec3 transformedPosition;
smooth in vec3 transformedNormal;

out vec4 colour;

void main()
{
	float d = distance(point.lightPos.xyz, transformedPosition);
	if (d > point.dist)
		colour = vec4(0.0, 0.0, 0.0, 1.0);
	else
	{
		vec4 ambientColour = material.ambient * point.ambient;

		vec3 lightVec = normalize(point.lightPos.xyz - transformedPosition);
		float s = max(dot(lightVec, transformedNormal), 0.0);
		vec4 diffuseColour = s * (material.diffuse * point.diffuse);

		vec3 toEye = normalize(eyePos - transformedPosition);
		vec3 r = reflect(-lightVec, transformedNormal);
		float t = pow(max(dot(r, toEye), 0.0), material.shininess);
		vec4 specularColour = t * (material.specular * point.specular);

		float A = point.attenuation.x + point.attenuation.y * d + point.attenuation.z * d * d;

		colour = ambientColour + ((diffuseColour + specularColour) / A);
		colour.a = 1.0;
	}
}