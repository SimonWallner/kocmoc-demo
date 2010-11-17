// Vertex Shader – file "minimal.vert"

#version 150 core

uniform mat4 rotation_matrix;

in  vec4 in_position;
in  vec3 in_color;
out vec3 ex_color;

void main(void)
{
	gl_Position = rotation_matrix * in_position;
	ex_color = in_color;
}
