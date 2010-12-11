// Vertex Shader – file "base.vert"

#version 150 core

in  vec3 inPosition;
in	vec2 inTexCoord0;

out vec2 texCoord0;

void main(void)
{
	gl_Position = vec4(inPosition, 1.0f);
	texCoord0 = inTexCoord0;
}
