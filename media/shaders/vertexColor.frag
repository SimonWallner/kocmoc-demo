// Fragment Shader – file "base.frag"

#version 150 core

in vec4 exColor;

out vec4 fragmentColor;

void main(void)
{
	fragmentColor = exColor;
}
