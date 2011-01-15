#include "FilmCamera.hpp"

#include <gtc/matrix_projection.hpp> 
#include <math.h>
#include "Property.hpp"

using namespace kocmoc;

FilmCamera::FilmCamera(vec3 _eyePosition, vec3 _targetPosition, vec3 _upVector) :
	eyePosition(_eyePosition),
	upVector(glm::normalize(_upVector)),
	angleOfView(KOCMOC_PI/2),
	nearPlane(util::Property("nearPlane")),
	farPlane(util::Property("farPlane"))
{
	targetVector = glm::normalize(_targetPosition - eyePosition);
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
	vec3 s = glm::normalize(glm::cross(targetVector, upVector));

	untranslatedViewMatrix = mat4(vec4(s, 0), vec4(upVector, 0), vec4(-targetVector, 0), vec4(0, 0, 0, 1.0f));
	untranslatedViewMatrix = glm::core::function::matrix::transpose(untranslatedViewMatrix);
	viewMatrix = glm::translate(untranslatedViewMatrix, -eyePosition);
		
	// as found in hearn & baker
	float x0 = 1/(tan(angleOfView/2));
	float y1 = (1/(tan(angleOfView/2))) * aspectRatio;
	float z2 = (nearPlane + farPlane)/(nearPlane - farPlane);
	float w2 = -1.0f;
	float z3 = (-2.0f * nearPlane * farPlane)/(nearPlane - farPlane);


	projectionMatrix = mat4(x0, 0, 0, 0,
							0, y1, 0, 0, 
							0, 0, z2, w2,
							0, 0, z3, 0);

	//projectionMatrix = mat4(0.2f, 0, 0, 0,
	//						0, aspectRatio*0.2f, 0, 0, 
	//						0, 0, 0.2f, 0,
	//						0, 0, 0, 1);
}

void FilmCamera::tumble(float horizontal, float vertical)
{
	// first horizontally
	vec3 s = glm::normalize(glm::cross(targetVector, upVector));
	targetVector = glm::normalize(targetVector + horizontal * s);

		
	// then vertically
//	s = glm::normalize(glm::cross(targetVector, upVector));
	targetVector = glm::normalize(targetVector + vertical * upVector);
	upVector = glm::normalize(glm::cross(s, targetVector));
}

void FilmCamera::dolly(vec3 direction)
{
	vec3 s = glm::normalize(glm::cross(targetVector, upVector));
	
	eyePosition += (direction.x * s);
	eyePosition += (direction.y * upVector);
	eyePosition += (direction.z * targetVector);

	//std::cout << "eye position: " << eyePosition.x << "|"<< eyePosition.y << "|"<< eyePosition.z << std::endl;
}


void FilmCamera::setAngleOfView(float radians)
{
	angleOfView = radians;
}

void FilmCamera::setFocalLength(float length)
{
	angleOfView = 2 * atan(35.0 / (2 * length));
}

void FilmCamera::rotate(float radians)
{
	// rotate up vector
	mat4 rotation = glm::gtx::transform::rotate(radians, targetVector);
	vec4 res = rotation * vec4(upVector, 1.0f);
	upVector = vec3(res);
	// std::cout << "up vector: " << upVector.x << " | "<< upVector.y << " | "<< upVector.z << std::endl;
} 

void FilmCamera::setGateInPixel(int _width, int _height)
{
	width = _width;
	height = _height;
	aspectRatio = (float)width/(float)height;
}

void FilmCamera::setFilterMarginInPixel(int _horizontalMargin, int _verticalMargin)
{
	horizontalMargin = _horizontalMargin;
	verticalMargin = _verticalMargin;
}