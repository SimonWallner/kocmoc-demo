// constants for atmospheric scattering
// SI units unless otherwise noted.

const float e = 2.71828182845904523536028747135266249775724709369995957f;
const float pi = 3.141592653589793238462643383279502884197169;

const float n = 1.0003; // refractive index of air
const float N = 2.545E25; // number of molecules per unit volume for air at
						// 288.15K and 1013mb (sea level -45 celsius)
const float pn = 0.035;	// depolatization factor for standard air

// wavelength of used primaries, according to preetham
const vec3 lambda = vec3(680E-9, 550E-9, 450E-9);

// mie stuff
// K coefficient for the primaries
const vec3 K = vec3(0.686f, 0.678f, 0.666f);
const float v = 4.0f;


// optical length at zenith for molecules
const float rayleighZenithLength = 8.4E3;
const float mieZenithLength = 1.25E3;
const vec3 up = vec3(0, 1, 0);

const float E = 100.0f;
const float sunAngularDiameterCos = 0.999956676946448443553574619906976478926848692873900859324f;

// earth shadow hack
const float cutoffAngle = 90.0;
const float steepness = 5.0f;





/**
 * Compute total rayleigh coefficient for a set of wavelengths (usually
 * the tree primaries)
 * @param lambda wavelength in m
 */
vec3 totalRayleigh(vec3 lambda)
{
	return (8 * pow(pi, 3) * pow(n*n - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(lambda, vec3(4)) * (6 - 7 * pn));
}

/** Reileight phase function as a function of cos(theta)
 */
float rayleighPhase(float cosTheta)
{
	// FIXME add braces

	return (3.0f / 16.0f*pi) * (1.0f + pow(cosTheta, 2));
}

/**
 * total mie scattering coefficient
 * @param lambda set of wavelengths in m
 * @param K corresponding scattering param
 * @param T turbidity, somewhere in the range of 0 to 20

 */
vec3 totalMie(vec3 lambda, vec3 K, float T)
{
	float c = (0.2f * T ) * 10E-18;
	return 0.434 * c * pi * pow((2 * pi) / lambda, vec3(v - 2)) * K;
}


/**
 * Henyey-Greenstein approximation as a function of cos(theta)
 * @param cosTheta 
 * @param g goemetric constant that defines the shape of the ellipse.
 */
float hgPhase(float cosTheta, float g)
{
	return (1.0f / (4.0f*pi)) * ((1.0f - g*g) / pow(1.0f + pow(g, 2) - 2.0f*g*cosTheta, 1.5));
}

float sunIntensity(float zenithAngle)
{
	return E;
	return max(0.0f, 1.0f - exp(-((cutoffAngle - zenithAngle)/steepness)));
}


