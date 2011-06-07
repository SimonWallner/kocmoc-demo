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
	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
	float brRed = totalRayleigh(red);
	float brGreen = totalRayleigh(green);
	float brBlue = totalRayleigh(blue);

	// mie coefficients
	float bmRed = totalMie(red, Kr);
	float bmGreen = totalMie(green, Kg);
	float bmBlue = totalMie(blue, Kb);



	// optical length
	float zenithAngle = acos(dot(up, normalize(worldSpacePosition - vec3(0, 0, 0))));
	float s = rayleighZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / pi), -1.253));
	//s = 0;

	// combined extinction factor	
	float Fr = exp(-(brRed + bmRed) * s);
	float Fg = exp(-(brGreen + bmGreen) * s);
	float Fb = exp(-(brBlue + bmBlue) * s);


	// in scattering

	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);

	float rPhase = rayleighPhase(cosTheta);
	float brThetaRed = brRed * rPhase;
	float brThetaGreen = brGreen * rPhase;
	float brThetaBlue = brBlue * rPhase;

	float bmThetaRed = bmRed * miePhase(cosTheta);
	float bmThetaGreen = bmGreen * miePhase(cosTheta);
	float bmThetaBlue = bmBlue * miePhase(cosTheta);		


	float LinR = ((brThetaRed + bmThetaRed) / (brRed + bmRed)) * sunIntensity * (1.0f - exp(-(brRed + bmRed) * s));
	float LinG = ((brThetaGreen + bmThetaGreen) / (brGreen + bmGreen)) * sunIntensity * (1.0f - exp(-(brGreen + bmGreen) * s));
	float LinB = ((brThetaBlue + bmThetaBlue) / (brBlue + bmBlue)) * sunIntensity * (1.0f - exp(-(brBlue + bmBlue) * s));

	// composition
//		fragmentColor0 = L0 * vec4(Fr, Fg, Fb, 1);
	//fragmentColor0 = + vec4(LinR, LinG, LinB, 0);

	//fragmentColor0.w = 1;
	
	fragmentColor1 = log(fragmentColor0.r * 0.2126f + fragmentColor0.g * 0.7152f + fragmentColor0.b * 0.0722f);  

	fragmentColor0 = vec4(2.1f);
}
