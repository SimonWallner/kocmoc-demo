#include "Camera.hpp"

#include <gtc/matrix_projection.hpp>

Camera::Camera(dvec3 _eyePosition, dvec3 _targetPosition, dvec3 _upVector) :
eyePosition(_eyePosition), targetPosition(_targetPosition), upVector(_upVector)
{	
}

Camera::~Camera()
{
}


void Camera::updateViewMatrix() 
{
	dmat4x4 translation;

	untranslatedViewMatrix = dmat4x4(1.0);

	viewMatrix = viewMatrix = glm::translate(untranslatedViewMatrix, glm::dvec3(0.0, 0.0, -5.0)); 
}


dmat4x4 Camera::getViewMatrix()
{
	return viewMatrix;
}

dmat4x4 Camera::getUntraslatedViewMatrix()
{
	return untranslatedViewMatrix;
}

dvec3 Camera::getEyeVector(){
	return eyePosition;
}
dvec3 Camera::getLookAtVector(){
	return targetPosition;
}

dvec3 Camera::getUpVector(){
	return upVector;
}