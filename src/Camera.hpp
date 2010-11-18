#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "common.hpp"

class Camera
{
public:
	/**
	 * Construct a new camera from the given params
	 * @param eyePosition The point from where the camera looks into the
	 * scene
	 * @param targetPosition The point the camera looks at
	 * @param upVector the up-vector of the camera
	 */
	Camera(vec3 eyePosition, vec3 targetPosition, vec3 upVector);

	virtual ~Camera(void);

	/** 
	 * Get the view matrix of this camera
	 * @return the view matrix of the camera
	 */
	mat4 getViewMatrix();

	/**
	 * Get the untranslated view matrix that only reflects the viewing
	 * direction. made for the skybox.
	 * @return the untranslated view matrix
	 */
	mat4 getUntraslatedViewMatrix();

	/**
	 * Update the view matrix of this camera with the current values. 
	 * You should call this method at the beginning of every frame. Camera
	 * parameters (eyePsition, targetPosition, etc.) my change during a 
	 * frame due to user inputs, but the viewMatrix remains constant until
	 * <code>updateViewMatrix()</code> is called.
	 * Also updates the untranslated view Matrix.
	 */
	void updateViewMatrix();

	/**
	 * Get position of the eye in form of vec3
	 */
	vec3 getEyeVector();

	vec3 getLookAtVector();

	vec3 getUpVector();
	
protected:
	mat4 viewMatrix;

	/** this is for the skybox and things alike */
	mat4 untranslatedViewMatrix;

	vec3 eyePosition;
	vec3 targetPosition;
	vec3 upVector;

};

#endif