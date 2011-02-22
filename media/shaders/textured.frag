// Fragment Shader – file "textured.frag"

#version 150 core

in vec2 exUV;

uniform sampler2D sTexture;

out vec4 fragmentColor0;

void main(void)
{
	fragmentColor0 = vec4(texture(sTexture, exUV).rgb, 1);
}
