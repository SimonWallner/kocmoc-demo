#ifndef KOCMOC_RENDERER_SHADOWO_MAP_HPP_
#define KOCMOC_RENDERER_SHADOWO_MAP_HPP_

#include <common.hpp>

namespace kocmoc 
{
	namespace renderer
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
}

#endif