// Fragment Shader – file "base.frag"

#version 150 core

#pragma include scattering.glsl
#pragma include luminance.glsl

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
	// cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = acos(max(0, dot(up, normalize(worldSpacePosition - vec3(0, 0, 0)))));
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

	float mPhase =  miePhase(cosTheta);
	float bmThetaRed = bmRed * mPhase;
	float bmThetaGreen = bmGreen * mPhase;
	float bmThetaBlue = bmBlue * mPhase;		


	float LinR = ((brThetaRed + bmThetaRed) / (brRed + bmRed)) * sunIntensity * (1.0f - exp(-(brRed + bmRed) * s));
	float LinG = ((brThetaGreen + bmThetaGreen) / (brGreen + bmGreen)) * sunIntensity * (1.0f - exp(-(brGreen + bmGreen) * s));
	float LinB = ((brThetaBlue + bmThetaBlue) / (brBlue + bmBlue)) * sunIntensity * (1.0f - exp(-(brBlue + bmBlue) * s));

	// composition
//		fragmentColor0 = L0 * vec4(Fr, Fg, Fb, 1);
	fragmentColor0 = vec4(LinR, LinG, LinB, 1);
	
//	fragmentColor0 = vec4(s);
//	fragmentColor0.a = 1;

	fragmentColor1 = logLuminance(fragmentColor0);  

}
