#ifndef ORTHO_CAM_HPP_
#define ORTHO_CAM_HPP_

#include "Camera.hpp"

namespace kocmoc
{
	class OrthoCam : public Camera
	{
	public:
		/**
		 * Construct a orthographic camera with a focus point
		 * and a direction. The focus point is the center of the
		 * viewing cube, and the direction is the direction of the
		 * view vector from eye towards focus.
		 */
		OrthoCam(vec3 focus, vec3 direction, vec3 upVector);
		~OrthoCam(void);

		// override fuctions from camera
		mat4 getViewMatrix() {return viewMatrix;};
		mat4 getProjectionMatrix() {return projectionMatrix;};
		mat4 getUntraslatedViewMatrix(){return mat4(1);};
		void updateMatrixes();

	private:
		vec3 focus;
		vec3 direction;
		vec3 upVector;
		float width;
		float height;
		float depth;

		mat4 viewMatrix;
		mat4 projectionMatrix;
		
	};
}
#endif ORTHO_CAM_HPP_