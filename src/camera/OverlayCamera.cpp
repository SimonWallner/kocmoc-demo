#include "OverlayCamera.hpp"

using namespace kocmoc::camera;
using glm::mat4;

OverlayCamera::OverlayCamera(int _width, int _height)
	: width(_width)
	, height(_height)
	, viewMatrix(mat4(1.0f))
{}

void OverlayCamera::updateMatrixes()
{
	projectionMatrix = mat4(2.0f/(float)width, 0, 0, 0,
							0, 2.0f/(float)height, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1);
}