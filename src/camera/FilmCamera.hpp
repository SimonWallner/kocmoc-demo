/*
 * A basic camera modelled after a real 35mm camera.
 */

#ifndef KOCMOC_CAMERA_FILM_CAMERA_HPP_
#define KOCMOC_CAMERA_FILM_CAMERA_HPP_

#include "Camera.hpp"

namespace kocmoc
{
	namespace camera
	{

		/**
		 * Implementation of a basic film camera
		 * @note a right-handed coordinate system is used throughout the whole 
		 * application. This is in sync with both opengl and collada.
		 */
		class FilmCamera : public Camera
		{
		public:
			/**
			 * Construct a new camera from the given params
			 * @param eyePosition The point from where the camera looks into the
			 * scene
			 * @param targetPosition The point the camera looks at
			 * @param upVector the up-vector of the camera
			 */
			FilmCamera(glm::vec3 eyePosition, glm::vec3 targetPosition, glm::vec3 upVector);

			void setPosition(glm::vec3 position);
			void setTargetPosition(glm::vec3 target);
			void setUpVector(glm::vec3 up);

			virtual ~FilmCamera();

			/**
			 * Set the film gate in pixel
			 * Set the film gate to specific size in pixels. This will mostly be the
			 * final output resolution, or the visual area in a letterboxed output.
			 * @param width The width of the gate
			 * @param height The height of the gate
			 */
			void setGateInPixel(int width, int height);

			/**
			 * The filter margin of the camera.
			 * A field wider than the final gate is rendered to provide some
			 * margin for post processing filters.
			 */
			void setFilterMarginInPixel(int horizontalMargin, int verticalMargin);

			int getFrameWidth(void);
			int getFrameHeight(void);
			int getGateWidth(void);
			int getGateHeight(void);

			/**
			 * Set the 35mm equivalent focal length of the lens. This should be
			 * calculated for the visible area i.e. the gate.
			 * @param The focal length as it would be in a standard 35mm film camera.
			 */
			void setFocalLength(float mm);

			/**
			 * The counterpart to setFocalLength. Directly set the horizontal AOV
			 * @param radians The horizontal AOV in radians.
			 */
			void setAngleOfView(float radians);
			float getAngleOfView(void) {return angleOfView;};

			// override fuctions from camera
			const glm::mat4 getViewMatrix();
			const glm::mat4 getProjectionMatrix();
			const glm::mat4 getUntraslatedViewMatrix();
			void updateMatrixes();

			/** get the cameras eye position */
			const glm::vec3 getPosition(void) {return eyePosition;};


			/**
			 * Tumble the camera around the eye position. input params have no unit!
			 * @param vertical The vertical rotation
			 * @param horizontal The horizontal rotation
			 */
			void tumble(float horizontal, float vertical);

			/**
			 * translate the camera. i.e. the postion and the target
			 */
			void dolly(glm::vec3 direction);

			/**
			 * rotate arround the target vector
			 * @param ccw rotation in radians
			 */
			void rotate(float radians);

		private:
			glm::mat4 viewMatrix;

			/** this is for the skybox and things alike */
			glm::mat4 untranslatedViewMatrix; 
			glm::mat4 projectionMatrix;

			/** The position of the camera/eye */
			glm::vec3 eyePosition;

			/** the vector looking from the eye towards the target */
			glm::vec3 targetVector;

			/** The vector starting at the eye towards the up direction
			 * i.e. the up vector away from the eye */
			glm::vec3 upVector;

			/** Near plane and far plane as seen from the camera, i.e. negative
			 * values, as we have a right-handed coordinate system and look down
			 * the negative z-axis. */
			float nearPlane;
			float farPlane;

			/** The aspect ration of the camera */
			float aspectRatio;

			/** The horizontal angle of view in radians */
			float angleOfView;
			
			/** the width and height of the gate. */
			int width;
			int height;

			/** horizontal and vertical filter margin */
			int horizontalMargin;
			int verticalMargin;
		};
	}
}

#endif