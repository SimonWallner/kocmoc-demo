// Fragment Shader – file "base.frag"

#version 150 core

in vec2 uv;

uniform sampler2D sImage;
uniform float opacity;

out vec4 fragmentColor0;

void main(void)
{
	fragmentColor0 = vec4(texture(sImage, uv).rgb, opacity);
}
