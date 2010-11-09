#include <fstream>

#include "shader.hpp"

Shader::Shader(const string &vertexShaderName, const string &fragmentShaderName) :
success(false)
{
	// Load the shader files
	string vertexShaderPath = SHADER_PATH_PREFIX + vertexShaderName;
	string vertexShaderSource;
	if (file_exists(vertexShaderPath)) {
		vertexShaderSource = read_file(vertexShaderPath);
	} else {
		cerr << "Vertex shader file " << vertexShaderPath <<" does not exist." << endl;
		return;
	}

	string fragmentShaderPath = SHADER_PATH_PREFIX + fragmentShaderName;
	string fragmentShaderSource;
	if (file_exists(fragmentShaderPath)) {
		fragmentShaderSource = read_file(fragmentShaderPath);
	} else {
		cerr << "Fragment shader file " << fragmentShaderPath <<" does not exist." << endl;
		return;
	}


	// Compile the shaders
	vertexShader = compile(GL_VERTEX_SHADER, vertexShaderSource);
	if (vertexShader == 0)
		return;

	get_errors();

	fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (fragmentShader == 0)
		return;

	get_errors();

	// Link the shaders into a program

	link();
	if (program == 0)
		return;

	success = true;
	get_errors();
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
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

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

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
