// Vertex Shader - file "base.vert"

#version 130

in  vec4 inPosition;
in	vec2 inTexCoord0;
in  vec3 inNormal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat3 normalMatrix;
uniform mat4 shadowPV;
//uniform vec3 inLightPosition;

out vec2 texCoord0;
out vec3 fragmentNormal;
out vec3 worldSpacePosition;
out vec3 shadowUV;


void main(void)
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * inPosition;
	worldSpacePosition = (modelMatrix * inPosition).xyz;
	fragmentNormal = normalMatrix * inNormal;
	texCoord0 = inTexCoord0;

	shadowUV = (shadowPV * modelMatrix * inPosition).xyz;
}


//	fragmentNormal = vec3(viewMatrix * modelMatrix * vec4(inNormal, 1.0f));
//	lightPosition = inLightPosition;
//	incidentlightDirection = vec3(0.0f, -1.0f, 0.0f);

