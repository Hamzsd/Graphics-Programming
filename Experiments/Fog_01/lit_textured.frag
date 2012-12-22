#version 330

layout (std140) uniform;

#define MAX_LIGHTS 10

#ifndef MATERIAL
#define MATERIAL
struct MaterialData
{
	vec4 emissive;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};
#endif

uniform Material
{
	MaterialData material;
};

uniform sampler2D tex;

#ifndef POINT_LIGHT
#define POINT_LIGHT
struct PointLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightPos;
	vec4 attenuation;
	float dist;
};
#endif

#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct SpotLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 lightPos;
	vec4 attenuation;
	vec4 lightDir;
	float power;
	float dist;
};
#endif

uniform struct FogParameters
{
	vec4 vFogColor; //Fog Colour
	float fStart; //This is only for linear fog
	float fEnd; //This is only for linear fog
	float fDensity; // for exp and exp2 equation 

	int iEquation; // 0 = linear, 1 = exp, 2 = exp2
} fogParams;

uniform DynamicLights
{
	PointLight points[MAX_LIGHTS];
	SpotLight spots[MAX_LIGHTS];
} dynamic;

uniform int numPoints;
uniform int numSpots;

uniform vec3 eyePos;

in vec3 transformedPosition;
in vec3 transformedNormal;
in vec2 texCoordOut;
in vec4 vEyeSpacePos;

out vec4 colour;

vec4 calculateLighting(in MaterialData material, in vec3 toEye, in vec3 transformedNormal);

vec4 calculatePoint(in MaterialData material, in vec3 toEye, in PointLight point, in vec3 transformedPosition, in vec3 transformedNormal);

vec4 calculateSpot(in MaterialData material, in vec3 toEye, in SpotLight spot, in vec3 transformedPosition, in vec3 transformedNormal);

float getFogFactor(FogParameters params, float fFogCoord);

void main()
{
	vec3 toEye = normalize(eyePos - transformedPosition);

	vec4 col = calculateLighting(material, toEye, transformedNormal);

	for (int i = 0; i < numPoints; ++i)
		col += calculatePoint(material, toEye, dynamic.points[i], transformedPosition, transformedNormal);

	for (int i = 0; i < numSpots; ++i)
		col += calculateSpot(material, toEye, dynamic.spots[i], transformedPosition, transformedNormal);

	col *= texture2D(tex, texCoordOut);

	col += material.emissive;

	//Add fog
	float fFogCoord = abs(vEyeSpacePos.z/vEyeSpacePos.w);
	colour = mix(col, fogParams.vFogColor, getFogFactor(fogParams, fFogCoord));
	
	//colour = col;
}