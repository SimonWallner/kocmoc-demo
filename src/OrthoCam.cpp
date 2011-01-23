#include "OrthoCam.hpp"
#include "Property.hpp"

using namespace kocmoc;

OrthoCam::OrthoCam(vec3 _focus, vec3 _direction, vec3 _upVector)
	: focus(_focus)
	, direction(_direction)
	, upVector(glm::normalize(_upVector))
	, width(util::Property("orthoWidth"))
	, height(util::Property("orthoHeight"))
	, depth(util::Property("orthoDepth"))
{}

OrthoCam::~OrthoCam(void)
{
}

void OrthoCam::updateMatrixes()
{
	vec3 s = glm::normalize(glm::cross(direction, upVector));
	vec3 u = glm::normalize(glm::cross(s, direction));

	mat4 untranslatedViewMatrix = mat4(vec4(s, 0), vec4(u, 0), vec4(direction, 0), vec4(0, 0, 0, 1.0f));
	untranslatedViewMatrix = glm::core::function::matrix::transpose(untranslatedViewMatrix);

	viewMatrix = glm::translate(untranslatedViewMatrix, -focus);

	projectionMatrix = mat4(1/width,	0,			0,			0,
							0,			1/height,	0,			0, 
							0,			0,			1/depth,	0,
							0,			0,			0,			1);
}

void OrthoCam::setFocus(vec3 _focus)
{
	focus = _focus;
}