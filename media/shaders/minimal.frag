// Fragment Shader – file "minimal.frag"

#version 150 core

in  vec3 ex_color;
out vec4 out_color;

void main(void)
{
	out_color = vec4(ex_color,1.0);
}
