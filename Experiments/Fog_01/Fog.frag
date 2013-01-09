#version 330

uniform vec4 vFogColour;
uniform float fDensity;

float getFogFactor(FogParameters params, float fFogCoord)
{
	float fResult = 0.0;
	fResult = exp(-pow(fDensity*fFogCoord, 2.0));

	fResult = 1.0-clamp(fResult, 0.0, 1.0);

	return fResult;
}