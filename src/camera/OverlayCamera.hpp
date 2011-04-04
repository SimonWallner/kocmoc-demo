#ifndef	KOCMOC_CAMERA_OVERLAY_CAMERA_HPP_
#define KOCMOC_CAMERA_OVERLAY_CAMERA_HPP_

#include "Camera.hpp"

#include <common.hpp>

namespace kocmoc
{
	namespace camera
	{

		class OverlayCamera : public Camera
		{
		public:
			OverlayCamera(int width, int height);
			~OverlayCamera(void);

			// override functions
			const glm::mat4 getViewMatrix() {return viewMatrix;};
			const glm::mat4 getProjectionMatrix() {return projectionMatrix;};
			const glm::mat4 getUntraslatedViewMatrix(){return viewMatrix;};
			void updateMatrixes();

		private:
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
			int width;
			int height;
		};
	}
}
#endif 