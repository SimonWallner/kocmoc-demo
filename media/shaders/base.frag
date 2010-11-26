// Fragment Shader – file "base.frag"

#version 150 core

in vec4 fragmentPos;
in vec2 texCoord0;
in vec3 fragmentNormal;
in vec3 lightDirection;
in vec3 incidentLightPosition;

uniform sampler2D sTex0;

out vec4 fragmentColor;

void main(void)
{
//	fragmentColor = texture2D(sTex0, texCoord0);
	fragmentColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
