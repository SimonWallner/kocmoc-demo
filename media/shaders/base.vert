// Vertex Shader - file "base.vert"

#version 150 core

#pragma include scattering.glsl

in  vec4 inPosition;
in	vec2 inTexCoord0;
in  vec3 inNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform vec3 sunDirection;
uniform float reileighCoefficient;
uniform float mieCoefficient;
uniform float mieDirectionalG;
uniform float turbidity;

out vec2 texCoord0;
out vec3 fragmentNormal;
out vec3 worldSpacePosition;
out vec3 shadowUV;
out vec3 sunE;


void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * inPosition;
	worldSpacePosition = (modelMatrix * inPosition).xyz;
	fragmentNormal = normalMatrix * inNormal;
	texCoord0 = inTexCoord0;


	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
	vec3 betaR = totalRayleigh(lambda) * reileighCoefficient;

	// mie coefficients
	vec3 betaM = totalMie(lambda, K, turbidity) * mieCoefficient;

	// optical length
	// cutoff angle at 90 to avoid singularity in next formula.
	float sunAngle = acos(max(0, dot(up, sunDirection)));
	// FIXME: remove code duplication, extract function.
	float sR = rayleighZenithLength / (cos(sunAngle) + 0.15 * pow(93.885 - ((sunAngle * 180.0f) / pi), -1.253));
	float sM = mieZenithLength / (cos(sunAngle) + 0.15 * pow(93.885 - ((sunAngle * 180.0f) / pi), -1.253));

	// combined extinction factor	
	vec3 Fex = exp(-(betaR * sR + betaM * sM));

	// in scattering

	float cosTheta = 1; // i.e. starring at the sun

	float rPhase = rayleighPhase(cosTheta);
	vec3 betaRTheta = betaR * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	vec3 betaMTheta = betaM * mPhase;


	vec3 Lin = ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0f - Fex);

	vec3 sunI = vec3(sunIntensity(dot(sunDirection, up)));
	sunE = sunI;// * Fex + sunI * Lin;

}


//	fragmentNormal = vec3(viewMatrix * modelMatrix * vec4(inNormal, 1.0f));
//	lightPosition = inLightPosition;
//	incidentlightDirection = vec3(0.0f, -1.0f, 0.0f);

