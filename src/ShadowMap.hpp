#ifndef SHADOWO_MAP_HPP_
#define SHADOWO_MAP_HPP_

#include "common.hpp"

namespace kocmoc 
{
	class ShadowMap
	{
	public:
		ShadowMap(int width, int height);
		~ShadowMap(void);

		const int width;
		const int height;

		GLuint getTextureHandle(void) {return textureHandle;};
		GLuint getFBOHandle(void) {return fboHandle;};

	private:
		GLuint fboHandle;
		GLuint textureHandle;
		GLuint depthbufferHandle;

		void check(void);
	};
}

#endif SHADOWO_MAP_HPP_