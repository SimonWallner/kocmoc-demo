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

	vec2 bentUV = uv / (barrelParam/(uv.s * uv.s + (uv.t / aspectRatio) * (uv.t / aspectRatio)  + 1) + 1-barrelParam);

	vec2 finalUV = bentUV / 2 + 0.5; // to [0, 1]

	fragmentColor0 = texture2D(sTex0, finalUV);
}
