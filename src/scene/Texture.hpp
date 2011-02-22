#ifndef KOCMOC_RENDERER_TEXTURE_HPP_
#define KOCMOC_RENDERER_TEXTURE_HPP_

#include <common.hpp>

#include <string>

namespace kocmoc
{
	namespace renderer
	{

		/**
		 * A wrapper to wrap the texture handle with semantic info
		 */
		class Texture
		{
		public:
			
			/** The GL texture object handle */
			const GLuint handle;

			/** The location in the shader, i.e. the sampler name */
			const std::string location;

			/** The texture unit number.
			 * Values like GL_TEXTURE0..n
			 */
			const GLint textureUnit;

			Texture(GLuint _handle, std::string _location, GLint _textureUnit)
				: handle(_handle)
				, location(_location)
				, textureUnit(_textureUnit)
			{};

		};
	}
}


#endif