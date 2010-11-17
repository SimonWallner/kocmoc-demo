// Vertex Shader – file "base.vert"

#version 150 core

in  vec3 inPosition;
in	vec2 inTexCoord0;
in  vec3 inNormal;


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 inlightPosition;


out vec4 fragmentPos;
out vec2 texCoord0;
out vec3 fragmentNormal;
out vec3 lightDirection;
out vec3 incidentLightPosition;

void main(void)
{
	fragmentPosition = modelMatrix * viewMatrix * projectionMatrix * in_position;
	fragmentNormal = modelMatrix * ViewMatrix * inNormal;
	texCoord0 = inTexCoord0;

	lightPosition = inLightPosition;
	incidentLightDirection = vec3(0.0f, -1.0f, 0.0f);
}
