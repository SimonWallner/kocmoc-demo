// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;

uniform sampler2D sTex0;
uniform sampler3D sColorLUT;
uniform int nonPlanarProjection;

out vec4 fragmentColor0;

void main(void)
{
	const float barrelParam = 0.5;
	const float aspectRatio = 16.0f/9.0f;

	vec4 baseColor;

	if (nonPlanarProjection == 1)
	{
		vec2 uv = texCoord0 * 2 - 1; // to [-1, 1]

	//	vec2 bentUV = uv;
	//	bentUV.s = uv.s / (barrelParam/((uv.t / aspectRatio) * (uv.t / aspectRatio) + 1) + 1-barrelParam);
	//	bentUV.t = uv.t / (barrelParam/(uv.s * uv.s + 1) + 1-barrelParam);

		vec2 bentUV = uv / (barrelParam/(uv.s * uv.s + (uv.t / aspectRatio) * (uv.t / aspectRatio) + 1) + 1-barrelParam);	
		vec2 finalUV = (bentUV / 1.5) / 2 + 0.5; // to [0, 1]


	// vignetting
		float attenuation = 2.0f;
		float power = 3;
	
		float delta = distance(vec2(0.0f, 0.0f), (bentUV / vec2(1.0f, aspectRatio))) / 2.5;
		float darkening = 1 - pow(delta, power) * attenuation;

		baseColor = texture2D(sTex0, finalUV) * darkening;
	}
	else
	{
		baseColor = texture(sTex0, texCoord0);
	}

	if (texCoord0.x > 0.5f)
		fragmentColor0 = texture(sColorLUT, vec3(0.2f, 0.5f, 0.7f));
	else
		fragmentColor0 = baseColor;
}
