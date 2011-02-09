#include "Context.hpp"
#include "Exception.hpp"
#include "Property.hpp"
#include "PropertiesFileParser.hpp"

using namespace kocmoc;

Context::Context(void)
{
	wireframeMode = false;
}

Context::~Context(void)
{
}

Context& Context::getInstance(void)
{
	static Context instance;
	return instance;
}

void Context::setupGLFW()
{
	width = util::Property("width");
	height = util::Property("height");
	
	int windowMode;
	if (util::Property("fullscreen"))
		windowMode = GLFW_FULLSCREEN;
	else
		windowMode = GLFW_WINDOW;

	if (!glfwInit())
		throw Exception("FATAL: Failed to initialize glfw!");

	glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 8);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);

	// Set flags so GLFW gives us a forward-compatible context.
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//getAvailableResolutions();
		
	if (glfwOpenWindow(width, height, 
		0, 0, 0, 0, // r, g, b, a
		24, 8, // depth, stencil
		windowMode) != GL_TRUE)
	{
			throw Exception("Failed to initialize OpenGL window.");
	}

	glfwSetWindowPos(200, 20);
	glfwSetWindowTitle("KOCMOC");

#ifdef _WINDOWS
	// Load extensions and OGL>1.1 functions with gl3w
	// http://github.com/skaslev/gl3w

	if (gl3wInit()) 
		throw Exception("failed to initialize OpenGL");
	if (!gl3wIsSupported(3, 2)) 
		throw Exception("OpenGL 3.2 not supported");

	cout << "OpenGL " << glGetString(GL_VERSION) << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	//GLenum err = glewInit();
	//if (GLEW_OK != err) {
	//	/* Problem: glewInit failed, something is seriously wrong. */
	//	cout <<  "Error: " << glewGetErrorString(err) << endl;
	//	throw Exception("Failed to init glew (glewInit)!");
	//}

#endif


}


void Context::getAvailableResolutions(){
	GLFWvidmode list[100];
	GLuint listsize = 100;

	for(GLuint i=0;i<listsize;i++){
		list[i].Width=0;
		list[i].Height=0;
	}

	glfwGetVideoModes(list, listsize);

	GLuint tempsize = 0;
	for(GLuint i=0;i<listsize;i++){
		if(list[i].Width != 0){
			cout<<"Width: "<<list[i].Width<<" Height: "<<list[i].Height<<endl;
			tempsize++;
		}
		else
			listsize=tempsize;
	}
}
void Context::testOpenGL()
{
	// query opengl version
	int major,minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    cout << "Loaded OpenGL Version " << major << "." << minor << endl;

	if (glGetError() != GL_NO_ERROR)
		cout << "An error occurred querying the OpenGL version: ";

	// Check if context is forward compatible

	int profile;
	// check if we have a core-profile
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
		cout << "Context is forward compatible." << endl;
	else 
		cout << "Context is NOT forward compatible." << endl;

	int error = glGetError();
	if (error != GL_NO_ERROR)
		cout << "An error occurred determining the fwcc support: " << endl;
}

void Context::setGLStates()
{
	//glClearColor(0.442047, 0.387623, 0.361867, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearColor(0.502052f, 0.736210f, 0.8492514f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
	if (util::Property("backFaceCulling"))
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	glClearDepth(1.0f);

	glLineWidth(2.0f);
	glPointSize(2.0f);
	glEnable(GL_LINE_SMOOTH);

	setWireframe(util::Property("wireframe"));	
}

void Context::swapBuffers()
{
	glfwSwapBuffers();
}

void Context::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Context::setWireframe(bool enable)
{
	wireframeMode = enable;
	cout << "toggle wireframe mode: " << wireframeMode << endl;

	if(wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

