#include <fstream>

#include "Shader.hpp"
#include "PropertiesFileParser.hpp"
#include "utility.hpp"

using namespace kocmoc;

Shader::Shader(const string &_vertexShaderName, const string &_fragmentShaderName) :
	vertexShaderName(_vertexShaderName),
	fragmentShaderName(_fragmentShaderName),
	success(false)
{
	util::PropertiesFileParser::GetInstance().getProperty("shadersRootFolder", &pathPrefix);
	create(vertexShaderName, fragmentShaderName);
}

void Shader::create(const string &vertexShaderFile, const string &fragmentShaderFile)
{
	success = false;

	// Load the shader files
	string vertexShaderPath = pathPrefix + vertexShaderName;
	string vertexShaderSource;
	if (util::file_exists(vertexShaderPath)) {
		vertexShaderSource = util::read_file(vertexShaderPath);
	} else {
		cerr << "Vertex shader file " << vertexShaderPath <<" does not exist." << endl;
		return;
	}

	string fragmentShaderPath = pathPrefix + fragmentShaderName;
	string fragmentShaderSource;
	if (util::file_exists(fragmentShaderPath)) {
		fragmentShaderSource = util::read_file(fragmentShaderPath);
	} else {
		cerr << "Fragment shader file " << fragmentShaderPath <<" does not exist." << endl;
		return;
	}


	// Compile the shaders
	vertexShader = compile(GL_VERTEX_SHADER, vertexShaderSource);
	if (vertexShader == 0)
		return;

	fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (fragmentShader == 0)
		return;

	// Link the shaders into a program
	link();
	if (programHandle == 0)
		return;

	success = true;
}

Shader::~Shader()
{
	destroy();
}

void Shader::reload()
{
	destroy();
	create(vertexShaderName, fragmentShaderName);
}

void Shader::destroy()
{
	glDeleteProgram(programHandle);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

GLuint Shader::compile (GLenum type, const string &source)
{
	// Create shader object

	GLuint shaderHandle = glCreateShader(type);

	if (shaderHandle == 0) {
		cerr << "Could not create shader object." << endl;
		return 0;
	}

	// Define shader source and compile

	const char* src = source.data();
	int len = source.size();

	glShaderSource(shaderHandle, 1, &src, &len);

	glCompileShader(shaderHandle);

	// Check for errors
	int status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Shader compilation failed." << endl;
		shader_log(shaderHandle);
	}

	return shaderHandle;    
}

void Shader::link(void)
{
	// Create program handle
	programHandle = glCreateProgram();

	// Attach shaders and link

	glAttachShader(programHandle, vertexShader);
	glAttachShader(programHandle, fragmentShader);

	// bind attribute and frag data locations to indexes
	glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_POSITION, VERTEX_ATTR_NAME_POSITION);
	glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_NORMAL, VERTEX_ATTR_NAME_NORMAL);
	glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_UV0, VERTEX_ATTR_NAME_UV0);
	glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_COLOR, VERTEX_ATTR_NAME_COLOR);
	glBindFragDataLocation(programHandle, 0, FRAGMENT_DATA_LOCATION_0_NAME);

	glLinkProgram(programHandle);

	// Check for problems
	int status;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &status);

	if (status != GL_TRUE)
	{
		cout << "Shader linking failed." << endl;
		program_log(programHandle);

		glDeleteProgram(programHandle);
		programHandle = 0;
	}
}

#define LOG_BUFFER_SIZE 8096

void Shader::program_log(GLuint programHandle)
{
	char logBuffer[LOG_BUFFER_SIZE];
	GLsizei length;

	logBuffer[0] = '\0';
	glGetProgramInfoLog(programHandle, LOG_BUFFER_SIZE, &length,logBuffer);

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
