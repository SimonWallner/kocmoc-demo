#include "OverlayCam.hpp"

using namespace kocmoc;

OverlayCam::OverlayCam(int _width, int _height)
	: width(_width)
	, height(_height)
	, viewMatrix(mat4(1.0f))
{}

void OverlayCam::updateMatrixes()
{
	projectionMatrix = mat4(2.0f/(float)width, 0, 0, 0,
							0, 2.0f/(float)height, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1);
}