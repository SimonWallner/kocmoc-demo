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
	float sunE = sunIntensity(dot(sunDirection, vec3(0.0f, 1.0f, 0.0f)));

	
	// extinction (absorbtion + out scattering)
	// rayleigh coefficients
	vec3 betaR = totalRayleigh(lambda) * reileighCoefficient;

	// mie coefficients
	vec3 betaM = totalMie(lambda, K, turbidity) * mieCoefficient;


	// optical length
	// cutoff angle at 90 to avoid singularity in next formula.
	float zenithAngle = acos(max(0, dot(up, normalize(worldSpacePosition - vec3(0, 0, 0)))));
	float sR = rayleighZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / pi), -1.253));
	float sM = mieZenithLength / (cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0f) / pi), -1.253));


	// combined extinction factor	
	vec3 Fex = exp(-(betaR * sR + betaM * sM));


	// in scattering

	float cosTheta = dot(normalize(worldSpacePosition - cameraPosition), sunDirection);

	float rPhase = rayleighPhase(cosTheta);
	vec3 betaRTheta = betaR * rPhase;

	float mPhase = hgPhase(cosTheta, mieDirectionalG);
	vec3 betaMTheta = betaM * mPhase;


	vec3 Lin = sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0f - Fex);

	// nightsky
	vec3 direction = normalize(worldSpacePosition - cameraPosition);
	float theta = acos(direction.y); // elevation --> y-axis, [-pi/2, pi/2]
	float phi = atan(direction.z, direction.x); // azimuth --> x-axis [-pi/2, pi/2]
	vec2 uv = vec2(phi, theta) / vec2(2*pi, pi) + vec2(0.5f, 0.0f);
	vec3 L0 = texture(sDiffuse, uv).rgb ;

	// composition

	if (cosTheta > sunAngularDiameterCos)
		L0 += sunE * Fex;

		
	fragmentColor0 = vec4(L0 + Lin, 1);
//	fragmentColor0 = vec4(uv, 0, 1);



	
//	fragmentColor0 = vec4(mPhase);
//	fragmentColor0.a = 1;

	fragmentColor1 = logLuminance(fragmentColor0);  

}
