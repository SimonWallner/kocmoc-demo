#ifndef KOCMOC_RENDERER_SHADER_HPP_
#define KOCMOC_RENDERER_SHADER_HPP_

#include <common.hpp>
#include <Symbol.hpp>

#include <string>
#include <vector>

#define VERTEX_ATTR_NAME_POSITION		"inPosition"
#define VERTEX_ATTR_NAME_NORMAL			"inNormal"
#define VERTEX_ATTR_NAME_UV0			"inTexCoord0"
#define VERTEX_ATTR_NAME_COLOR			"inColor"

#define FRAGMENT_DATA_LOCATION_0_NAME	"fragmentColor0"

#define DIFFUSE_SAMPLER_NAME			"sDiffuse"
#define SPECULAR_SAMPLER_NAME			"sSpecular"
#define NORMAL_SAMPLER_NAME				"sNormal"
#define SHADOW_SAMPLER_NAME				"sShadow"

namespace kocmoc
{
	namespace renderer
	{

		// TODO replace assert with useful exceptions!

		/**
		 * Basic shader class, taken from/built upon the RTR OpenGL 3 sample
		 */
		class Shader
		{
		public:

			struct VertexAttributeSemantic
			{
				/** the attribute name in the mesh */
				Symbol attributeName;

				/** the attribute location/name in the shader */
				std::string attributeLocation;

				/** the attribute index in the shader used while linking */
				GLuint attributeIndex;

				/** Construct a new instance with the fields above */
				VertexAttributeSemantic(Symbol _attributeName,
					std::string(_attributeLocation),
					GLuint _attributeIndex)
					: attributeName(_attributeName)
					, attributeLocation(_attributeLocation)
					, attributeIndex(_attributeIndex)
				{};
			};

			typedef std::vector<VertexAttributeSemantic> VertexAttributeSemanticList;

			/**
			 * Load and compile the files into a shader. All shaders are assumed
			 * to reside in the same folder and use a common path prefix.
			 *
			 * @param vertexShaderFile the name of  the vertex shader file
			 * @param fragmentShaderFile the name of the fragment shader file
			 */
			Shader(const std::string &vertexShaderFileName,
				const std::string &fragmentShaderFileName);

			~Shader();

			/**
			 * Upload the shader to the GPU.
			 *
			 * This is a prerequisite for most of the operations that use the shader.
			 * If the shader is already loaded, it is deleted and recreated.
			 *
			 * @return \c true	\b iff the shader has been compiled and linked
			 *					successfully and is ready to use.
			 */
			bool upload();

			/**
			 * Bind the shader to OpenGL.
			 *
			 * @pre The shader must be uploaded to the GPU.
			 */
			void bind() const;

			/**
			 * Unbind the shader
			 *
			 * @pre The shader must be uploaded to the GPU.
			 */
			void unbind() const;

			/**
			 * Query the location of a vertex attribute inside the shader.
			 *
			 * @param name The name of the attribute
			 * @return the attribute location.
			 *
			 * @pre The shader must be uploaded to the GPU.
			 */
			GLint get_attrib_location(const std::string &name) const;

			/**
			 * Query the location of a uniform inside the shader.
			 *
			 * @param name The name of the uniform
			 * @return the uniform location.
			 *
			 * @pre The shader must be uploaded to the GPU.
			 */
			GLint get_uniform_location(const std::string &name) const;


			/**
			 * Reload the shader.
			 *
			 * I.e. delete the program and create it again from source. 
			 *
			 * @pre The shader must be uploaded to the GPU.
			 */
			void reload();

			/**
			 * Add a vertex attribute semantic to the shader.
			 *
			 * A vertex attribute semantic defines how vertex attributes in meshes
			 * are linked to shader inputs. When uploadig a \RenderMesh these sematic
			 * bindings are lookup and set in the VAO accordingly during the 
			 * upload.
			 *
			 * Semantics are arbitrary but should at least contain a 'position' sematic
			 * to account for the drault position attribute of a \c PolyMesh.
			 *
			 * @param semantic the attribute semantic
			 */
			void addSemantic(VertexAttributeSemantic semantic);

			/**
			 * Retrieve a list of vertex attribute semantics.
			 */
			VertexAttributeSemanticList getSemantics(void) const {return vertexAttributeSemanticList;};

			bool getIsUploaded(void) const {return isUploaded;};

		private:

			/** Whether the shader is uploaded to the GPU or not.
			 * Most operations can only performed if this property is true
			 */
			bool isUploaded;

			void setParams(void);

			std::string pathPrefix;

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

			VertexAttributeSemanticList vertexAttributeSemanticList;
		};
	}
}

#endif