#version 330

vec4 calculateFog(in vec4 transformedEyePos, in vec4 fogColour, in float density, in vec4 col, in vec3 transformedPosition)
{
	//Variables needed for fog. 
	float fFogCoord, fDensity, fogFactor;
	vec4 vFogColour, waterCol, fogCol, colour;

	if (transformedPosition.y < 2.1)
	{
		//Add water
		fFogCoord = transformedEyePos.z;
		vFogColour = vec4(0.0, 0.0, 1.0, 1.0);
		fDensity = 0.1;
		fogFactor = exp(-pow(fDensity * fFogCoord, 2.0));
		fogFactor = 1.0-clamp(fogFactor, 0.0, 1.0);
		waterCol = mix(col, vFogColour, fogFactor);

		//Add fog to water
		fFogCoord = transformedEyePos.z;
		vFogColour = fogColour;
		fDensity = density;
		fogFactor = exp(-pow(fDensity * fFogCoord, 2.0));
		fogFactor = 1.0-clamp(fogFactor, 0.0, 1.0);
		fogCol = mix(col, vFogColour, fogFactor);
	}
	else 
	{
		//Add fog everywhere else
		fFogCoord = transformedEyePos.z;
		vFogColour = fogColour;
		fDensity = density;
		fogFactor = exp(-pow(fDensity * fFogCoord, 2.0));
		fogFactor = 1.0-clamp(fogFactor, 0.0, 1.0);
		fogCol = mix(col, vFogColour, fogFactor);
	}
	return colour = waterCol + fogCol;
}