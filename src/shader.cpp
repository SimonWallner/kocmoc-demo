#include <fstream>

#include "shader.hpp"

Shader::Shader(const string &path) :
_success(false)
{
	// Load the shader files

	string vertex_shader_source;
	if (file_exists(path+".vert")) {
		vertex_shader_source = read_file(path+".vert");
	} else {
		cerr << "Vertex shader file "
			<< path <<".vert does not exist." << endl;
		return;
	}

	string fragment_shader_source;
	if (file_exists(path+".frag")) {
		fragment_shader_source = read_file(path+".frag");
	} else {
		cerr << "Fragment shader file "
			<< path <<".frag does not exist." << endl;
		return;
	}

	// Compile the shaders

	_vertex_shader = compile(GL_VERTEX_SHADER, vertex_shader_source);
	if (_vertex_shader == 0)
		return;

	get_errors();

	_fragment_shader = compile(GL_FRAGMENT_SHADER, fragment_shader_source);
	if (_fragment_shader == 0)
		return;

	get_errors();

	// Link the shaders into a program

	link();
	if (program == 0)
		return;

	_success = true;
	get_errors();
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(_vertex_shader);
	glDeleteShader(_fragment_shader);
}

GLuint Shader::compile (GLenum type, const string &source)
{
	// Create shader object

	GLuint shader = glCreateShader(type);

	if (shader == 0) {
		cerr << "Could not create shader object." << endl;
		return 0;
	}

	// Define shader source and compile

	const char* src = source.data();
	int len = source.size();

	glShaderSource(shader, 1, &src, &len);

	glCompileShader(shader);

	// Check for errors

	int status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		cout << "Shader compilation failed." << endl;
		shader_log(shader);
	}

	get_errors();

	return shader;    
}

void Shader::link(void)
{
	// Create program handle
	program = glCreateProgram();

	// Attach shaders and link

	glAttachShader(program, _vertex_shader);
	glAttachShader(program, _fragment_shader);

	glLinkProgram(program);

	// Check for problems

	int status;

	glGetProgramiv(program, GL_LINK_STATUS, &status);

	if (status != GL_TRUE) {
		cout << "Shader linking failed." << endl;
		program_log(program);

		glDeleteProgram(program);
		program = 0;
	}

	get_errors();
}

#define LOG_BUFFER_SIZE 8096

void Shader::program_log(GLuint program)
{
	char logBuffer[LOG_BUFFER_SIZE];
	GLsizei length;

	logBuffer[0] = '\0';
	glGetProgramInfoLog(program, LOG_BUFFER_SIZE, &length,logBuffer);

	if (length > 0) {
		cout << logBuffer << endl;
	}
};

void Shader::shader_log(GLuint shader)
{
	char logBuffer[LOG_BUFFER_SIZE];
	GLsizei length;

	logBuffer[0] = '\0';
	glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, &length,logBuffer);

	if (length > 0) {
		cout << logBuffer << endl;
	}
};
