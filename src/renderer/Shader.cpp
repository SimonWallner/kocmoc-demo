#include "Shader.hpp"

#include <util/Property.hpp>
#include <util/util.hpp>

#include <fstream>
#include <cassert>

using namespace kocmoc::renderer;
using std::string;
using std::cerr;
using std::cout;
using std::endl;

Shader::Shader(const std::string &_vertexShaderName, const std::string &_fragmentShaderName) :
	vertexShaderName(_vertexShaderName),
	fragmentShaderName(_fragmentShaderName),
	isUploaded(false),
	pathPrefix(util::Property("shadersRootFolder"))
{}

bool Shader::upload()
{
	if (isUploaded)
		destroy();

	// Load the shader files
	string vertexShaderPath = pathPrefix + vertexShaderName;
	string vertexShaderSource;
	if (util::file_exists(vertexShaderPath)) {
		vertexShaderSource = util::read_file(vertexShaderPath);
	} else {
		cerr << "Vertex shader file " << vertexShaderPath <<" does not exist." << endl;
		return false;
	}

	string fragmentShaderPath = pathPrefix + fragmentShaderName;
	string fragmentShaderSource;
	if (util::file_exists(fragmentShaderPath)) {
		fragmentShaderSource = util::read_file(fragmentShaderPath);
	} else {
		cerr << "Fragment shader file " << fragmentShaderPath <<" does not exist." << endl;
		return false;
	}


	// Compile the shaders
	vertexShader = compile(GL_VERTEX_SHADER, vertexShaderSource);
	if (vertexShader == 0)
		return false;

	fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderSource);
	if (fragmentShader == 0)
		return false;

	// Link the shaders into a program
	link();
	if (programHandle == 0)
		return false;

	isUploaded = true;
	
	setParams();
	
	return isUploaded;
}

Shader::~Shader()
{
	destroy();
}

void Shader::reload()
{
	if(isUploaded)
	{
		std::cout << "--- reloading shader: [" << vertexShaderName << "/" << fragmentShaderName << "]" << std::endl;
		destroy();
		upload();
	}
}

void Shader::destroy()
{
	if (isUploaded)
	{
		glDeleteProgram(programHandle);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	isUploaded = false;
}

GLuint Shader::compile (GLenum type, const std::string &source)
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
	for (VertexAttributeSemanticList::const_iterator ci = vertexAttributeSemanticList.begin();
		ci != vertexAttributeSemanticList.end();
		ci++)
	{
		glBindAttribLocation(programHandle, ci->attributeIndex, ci->attributeLocation.c_str());
	}

	//glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_POSITION, VERTEX_ATTR_NAME_POSITION);
	//glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_NORMAL, VERTEX_ATTR_NAME_NORMAL);
	//glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_UV0, VERTEX_ATTR_NAME_UV0);
	//glBindAttribLocation(programHandle, VERTEX_ATTR_INDEX_COLOR, VERTEX_ATTR_NAME_COLOR);
	
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

void Shader::setParams()
{
	bind();
	
	GLint location;
	if ((location = get_uniform_location(DIFFUSE_SAMPLER_NAME)) > 0)
		glUniform1i(location, 0);
	if ((location = get_uniform_location(SPECULAR_SAMPLER_NAME)) > 0)
		glUniform1i(location, 1);
	if ((location = get_uniform_location(NORMAL_SAMPLER_NAME)) > 0)
		glUniform1i(location, 2);
	if ((location = get_uniform_location(SHADOW_SAMPLER_NAME)) > 0)
		glUniform1i(location, 3);

	unbind();
}

void Shader::addSemantic(VertexAttributeSemantic semantic)
{
	vertexAttributeSemanticList.push_back(semantic);
}

void Shader::bind() const
{
	assert(isUploaded);
	glUseProgram(programHandle);
}

void Shader::unbind() const
{
	assert(isUploaded);
	glUseProgram(0);
}

GLint Shader::get_attrib_location(const std::string &name) const
{
	assert(isUploaded);
	return glGetAttribLocation(programHandle, name.c_str());
}

GLint Shader::get_uniform_location(const std::string &name) const
{
	assert(isUploaded);
	GLint location = glGetUniformLocation(programHandle, name.c_str());
	//if (location < 0 && _DEBUG)
	//	cout << "uniform location: " << name << " not found!" << endl;
	return location;
}