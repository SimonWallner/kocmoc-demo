// Fragment Shader – file "vertexColor.frag"

#version 150 core

in vec4 exColor;

out vec4 fragmentColor0;

void main(void)
{
	fragmentColor0 = exColor;
}
