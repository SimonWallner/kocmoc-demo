#ifndef KOCMOC_CAMERA_ORTHO_CAMERA_HPP_
#define KOCMOC_CAMERA_ORTHO_CAMERA_HPP_

#include "Camera.hpp"

namespace kocmoc
{
	namespace camera
	{

		class OrthoCamera : public Camera
		{
		public:
			/**
			 * Construct a orthographic camera with a focus point
			 * and a direction. The focus point is the center of the
			 * viewing cube, and the direction is the direction of the
			 * view vector from eye towards focus.
			 */
			OrthoCamera(glm::vec3 focus, glm::vec3 direction, glm::vec3 upVector);
			~OrthoCamera(void);

			// override fuctions from camera
			glm::mat4 getViewMatrix() {return viewMatrix;};
			glm::mat4 getProjectionMatrix() {return projectionMatrix;};
			glm::mat4 getUntraslatedViewMatrix(){return glm::mat4(1);};
			void updateMatrixes();

			void setFocus(glm::vec3 focus);

		private:
			glm::vec3 focus;
			glm::vec3 direction;
			glm::vec3 upVector;
			float width;
			float height;
			float depth;

			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};
	}
}
#endif