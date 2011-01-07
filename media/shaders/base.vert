// Vertex Shader – file "base.vert"

#version 150 core

in  vec4 inPosition;
in	vec2 inTexCoord0;
in  vec3 inNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
//uniform vec3 inLightPosition;

out vec2 texCoord0;
out vec3 fragmentNormal;
//out vec3 lightPosition;
//out vec3 incidentlightDirection;


void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * inPosition;
	fragmentNormal = inNormal;
	texCoord0 = inTexCoord0;
}


//	fragmentNormal = vec3(viewMatrix * modelMatrix * vec4(inNormal, 1.0f));
//	lightPosition = inLightPosition;
//	incidentlightDirection = vec3(0.0f, -1.0f, 0.0f);

