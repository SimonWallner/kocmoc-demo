// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;
in vec3 fragmentNormal;
//in vec3 lightDirection;
//in vec3 incidentLightPosition;

uniform sampler2D sDiffuse;
uniform sampler2D sSpecular;
uniform sampler2D sNormal;

out vec4 fragmentColor0;

void main(void)
{

	const vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 1.0f));
	const vec4 ambientIntensity = vec4(0.05f, 0.05f, 0.05f, 1.0f);

	vec4 diffuse = vec4(1, 1, 1, 1);
	vec3 normal = texture2D(sNormal, texCoord0).xyz * 2 -1; // unpack to [-1, 1]
//	fragmentColor0 = vec4(normal, 1.0f);
//	fragmentColor0 = texture(sDiffuse, texCoord0);

	fragmentColor0 = diffuse * max(dot(lightDirection, normal), 0) + diffuse * ambientIntensity;
}
