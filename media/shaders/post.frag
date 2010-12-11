// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;

uniform sampler2D sTex0;

out vec4 fragmentColor;

void main(void)
{
	fragmentColor = texture2D(sTex0, texCoord0/2);
}
