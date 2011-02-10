#ifndef OVERLAY_CAM_HPP_
#define OVERLAY_CAM_HPP_

#include "common.hpp"
#include "Camera.hpp"

namespace kocmoc
{
	class OverlayCamera : public Camera
	{
	public:
		OverlayCamera(int width, int height);
		~OverlayCamera(void);

		// override functions
		mat4 getViewMatrix() {return viewMatrix;};
		mat4 getProjectionMatrix() {return projectionMatrix;};
		mat4 getUntraslatedViewMatrix(){return viewMatrix;};
		void updateMatrixes();

	private:
		mat4 viewMatrix;
		mat4 projectionMatrix;
		int width;
		int height;
	};
}
#endif 