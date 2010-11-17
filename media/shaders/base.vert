// Vertex Shader – file "base.vert"

#version 150 core

in  vec3 inPosition;
in	vec2 inTexCoord0;
in  vec3 inNormal;


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec3 inLightPosition;


out vec4 fragmentPosition;
out vec2 texCoord0;
out vec4 fragmentNormal;
out vec3 lightPosition;
out vec3 incidentlightDirection;


void main(void)
{
	fragmentPosition = modelMatrix * viewMatrix * projectionMatrix * vec4(inPosition, 1.0f);
	fragmentNormal = modelMatrix * viewMatrix * vec4(inNormal, 1.0f);
	texCoord0 = inTexCoord0;

	lightPosition = inLightPosition;
	incidentlightDirection = vec3(0.0f, -1.0f, 0.0f);
}
