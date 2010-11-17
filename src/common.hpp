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

#include "utility.hpp"


#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

using glm::dvec3;
using glm::dvec4;
using glm::dmat4x4;


#include <iostream>
#include <string>

using std::string;
using std::cerr;
using std::cout;
using std::endl;

#endif
