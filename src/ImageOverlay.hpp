#ifndef IMAGE_OVERLAY_HPP_
#define IMAGE_OVERLAY_HPP_

#include "common.hpp"
#include "Shader.hpp"

namespace kocmoc
{
	class ImageOverlay
	{
	public:
		ImageOverlay(std::string fileName, int width, int height);
		~ImageOverlay(void);

		void draw(void);

		void setOpacity(float opacity);

		void setupShader(void);

	private:
		GLuint textureHandle;
		int width;
		int height;

		
		Shader* shader;

		GLuint* vboHandles;
		GLuint vaoHandle;

		void createQuad(void);
		float opacity;
	};
}

#endif