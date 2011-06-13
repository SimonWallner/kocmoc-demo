/**
 * log luminance according to a formula from wikipedia
 * http://en.wikipedia.org/wiki/Luminance_(relative)
 */
float logLuminance(vec4 c)
{
	return log(c.r * 0.2126f + c.g * 0.7152f + c.b * 0.0722f);
}
