#version 330

const float LOG2 = 1.442695;
float s = gl_FragCoord.z / gl_FragCoord.w;
float fogFactor = exp2( -gl_Fog.density * 
								gl_Fog.density *
								z * 
								z *
								LOG2);
fogFactor = clamp(fogFactor, 0.0, 1.0);

glFragColour = mix(gl_Fog.color, finalColour, fogFactor);