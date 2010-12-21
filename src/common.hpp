#ifndef _COMMON_HPP_
#define _COMMON_HPP_

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

using glm::dvec3;
using glm::dvec4;
using glm::dmat4x4;
using glm::mat4;
using glm::vec3;
using glm::vec4;


#include <iostream>
#include <string>

using std::string;
using std::cerr;
using std::cout;
using std::endl;

#define KOCMOC_PI 3.141592653589793238462643383279502884197169399375
#define KOCMOC_DEBUG_VERBOSE_OUTPUT false

#define _DEBUG 1

#endif
