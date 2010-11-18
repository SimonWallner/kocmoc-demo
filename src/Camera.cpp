#include "Camera.hpp"

#include <gtc/matrix_projection.hpp>

Camera::Camera(vec3 _eyePosition, vec3 _targetPosition, vec3 _upVector) :
eyePosition(_eyePosition), targetPosition(_targetPosition), upVector(_upVector)
{	
}

Camera::~Camera()
{
}


void Camera::updateViewMatrix() 
{
	mat4 translation;

	untranslatedViewMatrix = mat4(1.0);

	viewMatrix = viewMatrix = glm::translate(untranslatedViewMatrix, glm::vec3(0.0, 0.0, -5.0)); 
}


mat4 Camera::getViewMatrix()
{
	return viewMatrix;
}

mat4 Camera::getUntraslatedViewMatrix()
{
	return untranslatedViewMatrix;
}

vec3 Camera::getEyeVector(){
	return eyePosition;
}
vec3 Camera::getLookAtVector(){
	return targetPosition;
}

vec3 Camera::getUpVector(){
	return upVector;
}