#version 330

uniform struct FogParameters
{
	vec4 vFogColor; //Fog Colour
	float fStart; //This is only for linear fog
	float fEnd; //This is only for linear fog
	float fDensity; // for exp and exp2 equation 

	int iEquation; // 0 = linear, 1 = exp, 2 = exp2
} fogParams;

float getFogFactor(FogParameters params, float fFogCoord)
{
	params.vFogColor = vec4(0.7, 0.7, 0.7, 1.0); //Fog Colour
	params.fStart = 10.0; //This is only for linear fog
	params.fEnd = 75.0; //This is only for linear fog
	params.fDensity = 0.04; // for exp and exp2 equation 

	params.iEquation = 2; // 0 = linear, 1 = exp, 2 = exp2

	float fResult = 0.0;
	if (params.iEquation == 0)
		 fResult = (params.fEnd - fFogCoord) / (params.fEnd - params.fStart);
	else if (params.iEquation == 1)
		 fResult = exp(-params.fDensity*fFogCoord);
	else if (params.iEquation == 2)
		 fResult = exp(-pow(params.fDensity*fFogCoord, 2.0));

	fResult = 1.0-clamp(fResult, 0.0, 1.0);

	return fResult;
}