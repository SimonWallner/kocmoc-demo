// constants for atmospheric scattering
// SI units unless otherwise noted.

const float e = 2.71828182845904523536028747135266249775724709369995957f;
const float pi = 3.141592653589793238462643383279502884197169;

const float n = 1.0003; // refractive index of air
const float N = 2.545E25; // number of molecules per unit volume for air at
						// 288.15K and 1013mb (sea level -45 celsius)
const float pn = 0.035;	// depolatization factor for standard air

// wavelength of used primaries
const float red = 680E-9;
const float green = 550E-9;
const float blue = 450E-9;

// mie stuff
const float T = 1.0f;
const float c = (0.6544 * T - 0.6510) * 10E-16;
const float Kr = 0.686f;
const float Kg = 0.678f;
const float Kb = 0.666f;
const float v = 4.0f;


/**
 * Compute total rayleigh coefficient for a given wavelength
 * @param lambda wavelength in m
 */
float totalRayleigh(float lambda)
{
	return (8 * pow(pi, 3) * pow(n*n - 1, 2) * (6 + 3 * pn)) / (3 * N * pow(lambda, 4) * (6 - 7 * pn));
}

/**
 * total mie scattering coefficient
 * @param lambda wavelength in m
 * @param K some scattering param
 */
float totalMie(float lambda, float K)
{
	return 0.434 * c * pi * pow((2 * pi) / lambda, v - 2) * K;
}
