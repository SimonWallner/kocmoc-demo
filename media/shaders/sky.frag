// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;
in vec3 fragmentNormal;
in vec3 worldSpacePosition;
in vec3 shadowUV;

uniform sampler2D sDiffuse;
uniform sampler2D sSpecular;
uniform sampler2D sNormal;
uniform sampler2D sShadow;

uniform mat3 normalMatrix;
uniform vec3 cameraPosition;
uniform vec3 sunDirection;

out vec4 fragmentColor0;
out float fragmentColor1;


#pragma include scattering.glsl

const float sunIntensity = 100.0f;


void main(void)
{

	// --- atmospheric scattering starts here ---



	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
	float brRed = totalRayleigh(red);
	float brGreen = totalRayleigh(green);
	float brBlue = totalRayleigh(blue);

	// mie coefficients
	float bmRed = totalMie(red, Kr);
	float bmGreen = totalMie(green, Kg);
	float bmBlue = totalMie(blue, Kb);


	// combined extinction factor
	float distance = 10.0E5;
	float Fr = exp(-(brRed + bmRed) * distance);
	float Fg = exp(-(brGreen + bmGreen) * distance);
	float Fb = exp(-(brBlue + bmBlue) * distance);



	// in scattering
	float g = 0.74;

	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);
	
	float brThetaRed = (3.0f / 16.0f*pi) * brRed * (1.0f + cosTheta*cosTheta);
	float brThetaGreen = (3.0f / 16.0f*pi) * brGreen * (1.0f + cosTheta*cosTheta);
	float brThetaBlue = (3.0f / 16.0f*pi) * brBlue * (1.0f + cosTheta*cosTheta);

	float bmThetaRed = (1.0f / 4.0f*pi) * bmRed * (pow(1.0f - g, 2) / pow(1.0f + g*g - 2.0f*g*cosTheta, 1.5));
	float bmThetaGreen = (1.0f / 4.0f*pi) * bmGreen * (pow(1.0f - g, 2) / pow(1.0f + g*g - 2.0f*g*cosTheta, 1.5));
	float bmThetaBlue = (1.0f / 4.0f*pi) * bmBlue * (pow(1.0f - g, 2) / pow(1.0f + g*g - 2.0f*g*cosTheta, 1.5));		


	float LinR = ((brThetaRed + bmThetaRed) / (brRed + bmRed)) * sunIntensity * (1.0f - exp(-(brRed + bmRed) * distance));
	float LinG = ((brThetaGreen + bmThetaGreen) / (brGreen + bmGreen)) * sunIntensity * (1.0f - exp(-(brGreen + bmGreen) * distance));
	float LinB = ((brThetaBlue + bmThetaBlue) / (brBlue + bmBlue)) * sunIntensity * (1.0f - exp(-(brBlue + bmBlue) * distance));

	// composition
//		fragmentColor0 = L0 * vec4(Fr, Fg, Fb, 1);
	fragmentColor0 = + vec4(LinR, LinG, LinB, 0);

	fragmentColor0.w = 1;
	
	fragmentColor1 = log(fragmentColor0.r * 0.2126f + fragmentColor0.g * 0.7152f + fragmentColor0.b * 0.0722f);  

}
