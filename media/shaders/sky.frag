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
uniform float reileighCoefficient;
uniform float mieCoefficient;
uniform float mieDirectionalG;
uniform float turbidity;

out vec4 fragmentColor0;
out float fragmentColor1;


void main(void)
{
	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
	float brRed = totalRayleigh(red) * reileighCoefficient;
	float brGreen = totalRayleigh(green) * reileighCoefficient;
	float brBlue = totalRayleigh(blue) * reileighCoefficient;

	// mie coefficients
	float bmRed = totalMie(red, Kr, turbidity) * mieCoefficient;
	float bmGreen = totalMie(green, Kg, turbidity) * mieCoefficient;
	float bmBlue = totalMie(blue, Kb, turbidity) * mieCoefficient;



	// optical length
	// cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = acos(max(0, dot(up, normalize(worldSpacePosition - vec3(0, 0, 0)))));
	float sR = rayleighZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / pi), -1.253));
	float sM = mieZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / pi), -1.253));


	// combined extinction factor	
	float Fr = exp(-(brRed * sR + bmRed * sM));
	float Fg = exp(-(brGreen * sR + bmGreen * sM));
	float Fb = exp(-(brBlue * sR + bmBlue * sM));


	// in scattering

	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);

	float rPhase = rayleighPhase(cosTheta);
	float brThetaRed = brRed * rPhase;
	float brThetaGreen = brGreen * rPhase;
	float brThetaBlue = brBlue * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	float bmThetaRed = bmRed * mPhase;
	float bmThetaGreen = bmGreen * mPhase;
	float bmThetaBlue = bmBlue * mPhase;		


	float LinR = sunIntensity * ((brThetaRed + bmThetaRed) / (brRed + bmRed)) * (1.0f - Fr);
	float LinG = sunIntensity * ((brThetaGreen + bmThetaGreen) / (brGreen + bmGreen)) * (1.0f - Fg);
	float LinB = sunIntensity * ((brThetaBlue + bmThetaBlue) / (brBlue + bmBlue)) * (1.0f - Fb);


	// composition

	vec4 L0 = vec4(0);
	if (cosTheta > sunAngularDiameterCos)
		L0 += vec4(vec3(sunIntensity), 0);

		
	fragmentColor0 = L0 + vec4(LinR, LinG, LinB, 1);
	
//	fragmentColor0 = vec4(mPhase);
//	fragmentColor0.a = 1;

	fragmentColor1 = logLuminance(fragmentColor0);  

}
