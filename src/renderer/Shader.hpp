#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include "common.hpp"

#define VERTEX_ATTR_INDEX_POSITION		(GLuint)0
#define VERTEX_ATTR_NAME_POSITION		"inPosition"

#define VERTEX_ATTR_INDEX_NORMAL		(GLuint)1
#define VERTEX_ATTR_NAME_NORMAL			"inNormal"

#define VERTEX_ATTR_INDEX_UV0			(GLuint)2
#define VERTEX_ATTR_NAME_UV0			"inTexCoord0"

#define VERTEX_ATTR_INDEX_COLOR			(GLuint)3
#define VERTEX_ATTR_NAME_COLOR			"inColor"

#define FRAGMENT_DATA_LOCATION_0_NAME	"fragmentColor0"

#define DIFFUSE_SAMPLER_NAME			"sDiffuse"
#define SPECULAR_SAMPLER_NAME			"sSpecular"
#define NORMAL_SAMPLER_NAME				"sNormal"
#define SHADOW_SAMPLER_NAME				"sShadow"

namespace kocmoc
{

	/**
	 * Baisc shader class, taken from/built upon the RTR OpenGL 3 sample
	 */
	class Shader
	{
	public:

		// Loads shaders from files and compiles them.
		// When path is "hello", the files "hello.frag" & "hello.vert"
		// will be loaded.
		/**
		 * Load and compile the files into a shader. All shaders are assumed
		 * to reside in the same folder and use a common path prefix.
		 *
		 * @param vertexShaderFile the name of  the vertex shader file
		 * @param fragmentShaderFile the name of the fragment shader file
		 */
		Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);

		~Shader();

		/**
		 * Bind the shader to OpenGL
		 */
		void bind() const
		{
			glUseProgram(programHandle);
		}

		/**
		 * Unbind the shader
		 */
		void unbind() const
		{
			glUseProgram(0);
		}

		/**
		 * Query the location of a vertex attribute inside the shader.
		 * @param name The name of the attribute
		 * @return the attribute location.
		 */
		GLint get_attrib_location(const std::string &name) const
		{
			return glGetAttribLocation(programHandle, name.c_str());
		}

		/**
		 * Query the location of a uniform inside the shader.
		 * @param name The name of the uniform
		 * @return the uniform location.
		 */
		GLint get_uniform_location(const std::string &name) const
		{
			GLint location = glGetUniformLocation(programHandle, name.c_str());
			//if (location < 0 && _DEBUG)
			//	cout << "uniform location: " << name << " not found!" << endl;
			return location;
		}

		/**
		 * A little cast helper.
		 * With this you can simply do "if (shader) {...}" to test if a
		 * shader has been compiled successfully.
		 */
		operator bool ()
		{
			return success;
		}

		void reload();

	private:

		void setParams(void);

		bool success;

		string pathPrefix;

		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint programHandle;

		std::string vertexShaderName;
		std::string fragmentShaderName;

		GLuint compile(GLenum type, const std::string &source);
		void link (void);

		void shader_log(GLuint shader);
		void program_log(GLuint programHandle);

		void create(const std::string &vertexShaderFile, const std::string &fragmentShaderFile);
		void destroy(void);
	};
}

#endif

