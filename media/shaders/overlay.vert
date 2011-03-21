// Vertex Shader – file "base.vert"

#version 130

in  vec3 inPosition;
in	vec2 inTexCoord0;

uniform mat4 projectionMatrix;

out vec2 uv;

void main(void)
{
	gl_Position = projectionMatrix * vec4(inPosition, 1.0f);
	uv = inTexCoord0;
}
