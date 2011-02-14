#ifndef KOCMOC_COMMON_HPP_
#define KOCMOC_COMMON_HPP_

#ifdef _WINDOWS
	#include <GL3/gl3w.h>
#else
	#define GL3_PROTOTYPES 1
	#include <GL3/gl3.h>
#endif

#define GLFW_NO_GLU
#include <GL/glfw.h>

#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <stdio.h>


#define KOCMOC_PI 3.141592653589793238462643383279502884197169399375
#define KOCMOC_DEBUG_VERBOSE_OUTPUT false

#endif
