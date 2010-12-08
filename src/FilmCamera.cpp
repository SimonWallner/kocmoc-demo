#include "FilmCamera.hpp"

#include <gtc/matrix_projection.hpp> 
#include <math.h>

FilmCamera::FilmCamera(vec3 _eyePosition, vec3 _targetPosition, vec3 _upVector) :
	eyePosition(_eyePosition),
	upVector(glm::normalize(_upVector))
{
	targetVector = glm::normalize(_targetPosition - eyePosition);
	nearPlane = -0.1f;
	farPlane = -1000.0f;
}

FilmCamera::~FilmCamera()
{
	// TODO Auto-generated destructor stub
}

mat4 FilmCamera::getProjectionMatrix()
{
	return projectionMatrix;
}

mat4 FilmCamera::getViewMatrix()
{
	return viewMatrix;
}

mat4 FilmCamera::getUntraslatedViewMatrix()
{
	return untranslatedViewMatrix;
}


void FilmCamera::updateMatrixes() 
{
	vec3 s = glm::normalize(glm::cross(-targetVector, vec3(0, 1, 0)));
	vec3 u = glm::normalize(glm::cross(s, -targetVector));

	untranslatedViewMatrix = mat4(vec4(s, 0), vec4(u, 0), vec4(-targetVector, 0), vec4(0, 0, 0, 1.0f));
	viewMatrix = glm::translate(untranslatedViewMatrix, -eyePosition);
		
	// as found in hearn & baker
	float x0 = (1/(tan(KOCMOC_PI/4))) / (16.0f/9.0f);
	float y1 = 1/(tan(KOCMOC_PI/4));
	float z2 = (nearPlane + farPlane)/(nearPlane - farPlane);
	float w2 = -1.0f;
	float z3 = (-2.0f * nearPlane * farPlane)/(nearPlane - farPlane);

	projectionMatrix = mat4(x0, 0, 0, 0,
							0, y1, 0, 0, 
							0, 0, z2, w2,
							0, 0, z3, 0);
}

void FilmCamera::tumble(float vertical, float horizontal)
{
	// FIXME: evil hack !!!! it only moves the target along the view plane
	// no true rotation.

	// first horizontal:
	vec3 s = glm::normalize(glm::cross(targetVector, upVector));
	
	targetVector += s * -horizontal;
	targetVector = glm::normalize(targetVector);


	// then vertical
	s = glm::normalize(glm::cross(targetVector, upVector));
	vec3 u = glm::normalize(glm::cross(targetVector, s));

	targetVector += u * vertical;
	targetVector = glm::normalize(targetVector);

	// update the up vector
	//upVector = glm::normalize(glm::cross(s, targetVector));
}

void FilmCamera::dolly(vec3 direction)
{
	vec3 s = glm::normalize(glm::cross(targetVector, upVector));
	
	eyePosition += (-direction.x * s);
	eyePosition += (direction.y * upVector);
	eyePosition += (-direction.z * targetVector);
}