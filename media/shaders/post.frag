// Fragment Shader – file "base.frag"

#version 150 core

in vec2 texCoord0;

uniform sampler2D sTex0;
uniform sampler3D sColorLUT;
uniform int nonPlanarProjection;
uniform int colorCorrection;
uniform int vignetting;
uniform ivec2 dimension;
uniform float angleOfView;

uniform float logLuminance;
uniform float evBias;
uniform float logContrast;

out vec4 fragmentColor0;

vec4 tonemap(vec4 hdr)
{
	float contrast = 6.0f;
	float bias = 0.5f;
	float stretch = 1.0f;

	vec3 c = hdr.rgb;
	float low = exp(logLuminance - bias - contrast/2);
	float high = exp(logLuminance - bias + contrast/2);



	vec3 ldr = (hdr.rgb - low) / (high - low);
	return vec4(ldr, hdr.a);

}

void main(void)
{
//	const float barrelParam = 0.7;
	const float barrelParam = 0.8;
	float aspectRatio = dimension.x / dimension.y;

	vec4 color;

	if (nonPlanarProjection == 1)
	{

		float scale = tan(angleOfView/2 * barrelParam);		

		vec2 normalizedUV = (texCoord0 * 2 - 1) / vec2(1, aspectRatio); // to [-1, 1] and square pixels
	

		vec2 angle = normalizedUV * (angleOfView/2 * barrelParam);

		vec2 bentUV = angle / (length(angle) / tan(length(angle))) / scale;
//		bentUV = angle;


		vec2 finalUV = (bentUV * vec2(1, aspectRatio)) / 2 + 0.5f;

		// multi sampling...
		vec2 msaaOffset = 0.5f * 1.0f/dimension;
		vec4 c = texture(sTex0, finalUV + msaaOffset);
		c += texture(sTex0, finalUV + msaaOffset * vec2(-1.0f, 1.0f));
		c += texture(sTex0, finalUV + msaaOffset * vec2(1.0f, -1.0f));
		c += texture(sTex0, finalUV + msaaOffset * vec2(-1.0f, -1.0f));
//		c += texture(sTex0, finalUV) * 6;

		color = c / 4.0f;
	}
	else
		color = texture(sTex0, texCoord0);


	if (vignetting == 1)
	{
		float attenuation = 3.0f;
		float power = 4.7;
	
		float delta = distance(texCoord0, vec2(0.5f, 0.5f));
		float darkening = 1 - pow(delta, power) * attenuation;

		color = color * vec4(darkening.xxx, 1);
	}


	// hdr tonemapping 
	color = tonemap(color);

	
	if (colorCorrection == 1)
		fragmentColor0 = texture(sColorLUT, color.rbg + 1.0f/64.0f); // need to change channels in the lookup
	else 
		fragmentColor0 = color;
}
