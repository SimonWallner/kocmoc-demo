// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;
in vec3 fragmentNormal;
in vec3 worldSpacePosition;
in vec3 shadowUV;

uniform sampler2D sDiffuse;
uniform sampler2D sSpecular;
uniform sampler2D sNormal;
uniform sampler2D sShadow;

uniform mat3 normalMatrix;
uniform vec3 cameraPosition;

out vec4 fragmentColor0;

void main(void)
{
	vec3 sunDirection = normalize(vec3(1.0, 1.0, 1.0));
	vec3 sunIntensity = vec3(1.0, 1.0, 1.0);
	float g = -0.75;
	float kR = 0;
	float kM = 0;
	float pi = 3.141592653589793238462643383279502884197169;

	float red = 600E-9;
	float green = 550E-9;
	float blue = 450E-9;
	

	vec3 viewingDirection = normalize(worldSpacePosition - cameraPosition);

	fragmentColor0 = vec4(max(0, dot(sunDirection, viewingDirection)), 0, 0, 1);
	
//	fragmentColor0 = vec4(.4, 0.8, 1, 1);

}
