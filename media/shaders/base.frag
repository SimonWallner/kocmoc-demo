// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;
in vec3 fragmentNormal;
in vec3 worldSpacePosition;
//in vec3 lightDirection;
//in vec3 incidentLightPosition;

uniform sampler2D sDiffuse;
uniform sampler2D sSpecular;
uniform sampler2D sNormal;

uniform mat4 normalMatrix;
uniform vec3 cameraPosition;

out vec4 fragmentColor0;

void main(void)
{

	const vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 1.0f));
	const vec4 ambientIntensity = vec4(0.01f, 0.01f, 0.01f, 1.0f);
	const float shinyness = 1;
	

	vec4 diffuseColor = texture(sDiffuse, texCoord0);//vec4(1, 1, 1, 1);
	vec4 specularColor = vec4(1, 0, 0, 1);//vec4(texture(sSpecular, texCoord0).rgb, 1);
	vec3 normal = (texture2D(sNormal, texCoord0).xyz * 2 -1); // unpack to [-1, 1], 


	vec4 transformed = normalize((normalMatrix * vec4(normal, 1)) * vec4(-1, 1, -1, 1)); // and flip strangely...???

	vec4 ambientTerm = diffuseColor * ambientIntensity;
	vec4 diffuseTerm = diffuseColor * max(dot(lightDirection, transformed.xyz), 0);


	vec3 viewVector = cameraPosition * vec3(-1, 1, 1) - worldSpacePosition* vec3(-1, 1, 1);
	vec3 reflected = reflect(lightDirection, transformed.xyz);

	vec4 specularTerm = specularColor * pow(max(dot(normalize(lightDirection), reflected), 0), shinyness);

	fragmentColor0 = ambientTerm + diffuseTerm + specularTerm;
	fragmentColor0 = ambientTerm + diffuseTerm;
//	fragmentColor0 = specularTerm;
//	fragmentColor0 = vec4(transformed.xyz, 1);
//	fragmentColor0 = vec4(fragmentNormal, 1) + ambientTerm;
//	fragmentColor0 = vec4(worldSpacePosition * vec3(-1, 1, 1), 1);
//	fragmentColor0 = vec4(cameraPosition* vec3(-1, 1, 1), 1);

}
