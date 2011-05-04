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

void main(void)
{

	// --- atmospheric scattering starts here ---
		// unit is m, unless otherwise noted.

		float e = 2.71828182845904523536028747135266249775724709369995957f;
		float pi = 3.141592653589793238462643383279502884197169;

		float n = 1.0003; // refractive index of air
		float N = 2.545E25; // number of molecules per unit volume for air at
							// 288.15K and 1013mb (sea level -45 celsius)
		float pn = 0.035;	// depolatization factor for standard air

		// wavelength in m
		float red = 680E-9;
		float green = 550E-9;
		float blue = 450E-9;

		// mie stuff
		float T = 1.0f;
		float c = (0.6544 * T - 0.6510) * 10E-16;
		float Kr = 0.686f;
		float Kg = 0.678f;
		float Kb = 0.666f;
		float v = 4.0f;


		// extinction (absorbtion + out scattering)
		float brRed = (8 * pow(pi, 3) * pow(n*n - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(red, 4) * (6 - 7 * pn));
		float brGreen = (8 * pow(pi, 3) * pow(n*n - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(green, 4) * (6 - 7 * pn));
		float brBlue = (8 * pow(pi, 3) * pow(n*n - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(blue, 4) * (6 - 7 * pn));
		
		float bmRed = 0.434 * c * pi * pow((2 * pi) / red, v - 2) * Kr;
		float bmGreen = 0.434 * c * pi * pow((2 * pi) / green, v - 2) * Kg;
		float bmBlue = 0.434 * c * pi * pow((2 * pi) / blue, v - 2) * Kb;

			
		float distance = 10.0E5;;
		float Fr = exp(-(brRed + bmRed) * distance);
		float Fg = exp(-(brGreen + bmGreen) * distance);
		float Fb = exp(-(brBlue + bmBlue) * distance);

		// in scattering

		float sunIntensity = 0.5f;
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
	
//	fragmentColor0 = vec4(.4, 0.8, 1, 1);

}
