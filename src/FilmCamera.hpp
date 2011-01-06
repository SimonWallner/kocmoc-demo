/*
 * A basic camera modelled after a real 35mm camera.
 */

#ifndef FILM_CAMERA_HPP_
#define FILM_CAMERA_HPP_

#include "Camera.hpp"

namespace kocmoc
{

	class FilmCamera : public Camera
	{
	public:
		/**
		 * Construct a new camera from the given params
		 * @param eyePosition The point from where the camera looks into the
		 * scene
		 * @param targetPosition The point the camera looks at
		 * @param upVector the up-vector of the camera
		 * @param aspectRatio the aspect ration
		 */

		FilmCamera(vec3 eyePosition, vec3 targetPosition, vec3 upVector, float aspectRation);

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
		void setFilterMargininPixel(int horizontalMargin, int verticalMargin);

		/**
		 * Set the 35mm equivalent focal length of the lens.
		 * @param The focal length as it would be in a standard 35mm film camera.
		 */
		void setFocalLength(float mm);

		/**
		 * The counterpart to setFocalLength. Directly set the horizontal AOV
		 * @param radians The horizontal AOV in radians.
		 */
		void setAngleOfView(float radians);

		// override fuctions from camera
		mat4 getViewMatrix();
		mat4 getProjectionMatrix();
		mat4 getUntraslatedViewMatrix();
		void updateMatrixes();

		//TODO: position, orientation, movement

		/**
		 * Rotate the camera around the eye position. input params have no unit!
		 * @param vertical The vertical rotation
		 * @param horizontal The horizontal rotation
		 */
		void tumble(float horizontal, float vertical);

		/**
		 * translate the camera. i.e. the postion and the target
		 */
		void dolly(vec3 direction);
	
	
		//TODO: focus, vertigo

		//TODO: color correction post thingys

	private:
		mat4 viewMatrix;
		mat4 untranslatedViewMatrix; /** this is for the skybox and things alike */
		mat4 projectionMatrix;

		vec3 eyePosition;
		vec3 targetVector;
		vec3 upVector;

		float nearPlane;
		float farPlane;
		float aspectRatio;

		/** The angle of view in radians */
		float angleOfView;
	};
}

#endif /* FILM_CAMERA_HPP_ */
