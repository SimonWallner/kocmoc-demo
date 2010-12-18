// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;

uniform sampler2D sTex0;

out vec4 fragmentColor0;

void main(void)
{
	const float barrelParam = 0.5;
	const float aspectRatio = 16.0f/9.0f;

	vec2 uv = texCoord0 * 2 -1; // to [-1, 1]

//	vec2 bentUV = uv;
//	bentUV.s = uv.s / (barrelParam/((uv.t / aspectRatio) * (uv.t / aspectRatio) + 1) + 1-barrelParam);
//	bentUV.t = uv.t / (barrelParam/(uv.s * uv.s + 1) + 1-barrelParam);

	vec2 bentUV = uv / (barrelParam/(uv.s * uv.s + (uv.t / aspectRatio) * (uv.t / aspectRatio) + 1) + 1-barrelParam);	
	vec2 finalUV = bentUV / 2 + 0.5; // to [0, 1]


// vignetting
	float attenuation = 2.0f;
	float power = 3;
	
	float delta = distance(vec2(0.0f, 0.0f), (bentUV / vec2(1.0f, aspectRatio))) / 1.5;
	float darkening = 1 - pow(delta, power) * attenuation;




	fragmentColor0 = texture2D(sTex0, finalUV) * darkening;

}
