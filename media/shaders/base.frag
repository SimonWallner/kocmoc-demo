// Fragment Shader – file "base.frag"

#version 150 core

#pragma include scattering.glsl

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
	const float ambientIntensity = sunIntensity * 0.1f;

	vec3 diffuseColor = vec3(0.5f);//texture(sDiffuse, texCoord0).rgb;
	vec3 ambientTerm = diffuseColor * ambientIntensity;


	vec3 specularColor = texture(sSpecular, texCoord0).rgb;
	float shinyness = texture(sSpecular, texCoord0).a * 50.0f + 5.0f;
	vec3 normal = texture(sNormal, texCoord0).xyz * 2 - 1; // unpack to [-1, 1], 
	vec3 transformed = normalize(normalMatrix * (normal * vec3(-1, 1, -1))); // and flip strangely...???
	transformed = fragmentNormal;

	float diffuseFactor =  sunIntensity * max(dot(sunDirection, transformed.xyz), 0);
	vec3 diffuseTerm = diffuseColor * diffuseFactor;
	//diffuseTerm = vec3(diffuseFactor);


	vec3 viewVector = normalize(cameraPosition - worldSpacePosition);
//	vec3 reflected = normalize(reflect(sunDirection, transformed.xyz));
//	vec3 specularTerm = diffuseFactor * specularColor * pow(max(dot(viewVector, reflected), 0), shinyness);
//	vec4 L0 = vec4(ambientTerm + diffuseTerm + specularTerm, 1);

	vec4 L0 = vec4(diffuseTerm + ambientTerm, 1);


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

		
	float distance = length(worldSpacePosition - cameraPosition);
	float Fr = exp(-(brRed + bmRed) * distance);
	float Fg = exp(-(brGreen + bmGreen) * distance);
	float Fb = exp(-(brBlue + bmBlue) * distance);

	// in scattering


	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);
	
	float rPhase = rayleighPhase(cosTheta);
	float brThetaRed = brRed * rPhase;
	float brThetaGreen = brGreen * rPhase;
	float brThetaBlue = brBlue * rPhase;

	float mPhase =  hgPhase(cosTheta);
	float bmThetaRed = bmRed * mPhase;
	float bmThetaGreen = bmGreen * mPhase;
	float bmThetaBlue = bmBlue * mPhase;	

	float LinR = ((brThetaRed + bmThetaRed) / (brRed + bmRed)) * sunIntensity * (1.0f - exp(-(brRed + bmRed) * distance));
	float LinG = ((brThetaGreen + bmThetaGreen) / (brGreen + bmGreen)) * sunIntensity * (1.0f - exp(-(brGreen + bmGreen) * distance));
	float LinB = ((brThetaBlue + bmThetaBlue) / (brBlue + bmBlue)) * sunIntensity * (1.0f - exp(-(brBlue + bmBlue) * distance));

	// composition
//		fragmentColor0 = L0 * vec4(Fr, Fg, Fb, 1);
	fragmentColor0 = L0 * vec4(Fr, Fg, Fb, 1) + vec4(LinR, LinG, LinB, 0) * 5;
//	fragmentColor0 = vec4(distance);

	fragmentColor0.w = 1;

	
	fragmentColor1 = log(fragmentColor0.r * 0.2126f + fragmentColor0.g * 0.7152f + fragmentColor0.b * 0.0722f);  
}
