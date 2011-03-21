// Fragment Shader – file "vertexColor.frag"

#version 130

in vec4 exColor;

out vec4 fragmentColor0;

void main(void)
{
	fragmentColor0 = exColor;
}
