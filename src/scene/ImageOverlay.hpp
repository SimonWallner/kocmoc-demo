#ifndef KOCMOC_SCENE_IMAGE_OVERLAY_HPP_
#define KOCMOC_SCENE_IMAGE_OVERLAY_HPP_

#include <common.hpp>

namespace kocmoc
{
	namespace camera
	{
		class Camera;
	}

	namespace renderer
	{
		class Shader;
	}

	namespace scene
	{

		class ImageOverlay
		{
		public:
			ImageOverlay(std::string fileName, int width, int height, camera::Camera* OverlayCam);
			~ImageOverlay(void);

			void draw(void);

			void setOpacity(float opacity);

			void setupShader(camera::Camera* overlayCamera);

		private:
			GLuint textureHandle;
			int width;
			int height;

			
			renderer::Shader* shader;

			GLuint* vboHandles;
			GLuint vaoHandle;

			void createQuad(void);
			float opacity;
		};
	}
}

#endif