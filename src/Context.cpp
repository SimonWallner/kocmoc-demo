#include "Context.hpp"
#include "Exception.hpp"
#include "ConfigFileParser.hpp"

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
	int width = ConfigFileParser::GetInstance().width;
	int height = ConfigFileParser::GetInstance().height;
	int windowMode;
	if (ConfigFileParser::GetInstance().fullscreen)
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
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

	

    //glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
	
	//glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
//	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	getAvailableResolutions();
		
	if (glfwOpenWindow(width, height, 
		0, 0, 0, 0, // r, g, b, a
		24, 8, // depth, stencil
		windowMode) != GL_TRUE)
	{
			throw Exception("Failed to initialize OpenGL window.");
	}

	glfwSetWindowPos(200, 20);
	glfwSetWindowTitle("KOCMOC");

	// Load extensions and OGL>1.1 functions with GLEW

//	GLenum err = glewInit();
//	if (GLEW_OK != err) {
//		/* Problem: glewInit failed, something is seriously wrong. */
//		cout <<  "Error: " << glewGetErrorString(err) << endl;
//		throw Exception("Failed to init glew (glewInit)!");
//	}
//
//    // Clear errors. GLFW setup seems to cause an opengl error while setup.
//	int error = glGetError();
//	if (error != GL_NO_ERROR)
//		cout << "post init error: " << gluErrorString(error) << endl;
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
			//cout<<"Width: "<<list[i].Width<<" Height: "<<list[i].Height<<endl;
			tempsize++;
		}
		else
			listsize=tempsize;
	}
}
void Context::testOpenGL()
{
	// query opengl version
	{
	
	int major,minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    cout << "Loaded OpenGL Version " << major << "." << minor << endl;

	//cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
		
		int error = glGetError();
		if (glGetError() != GL_NO_ERROR)
		{
			cout << "An error occured querying the OpenGL version: ";
			cout << gluErrorString(error) << endl;
		}

	}

	// Check if context is forward compatible
	{
		/*
		int profile;
		// check if we have a core-profile
		glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
		if (profile == GL_CONTEXT_CORE_PROFILE_BIT)
			cout << "Context is forward compatible." << endl;
		else 
			cout << "Context is NOT forward compatible." << endl;

		int error = glGetError();
		if (error != GL_NO_ERROR)
			cout << "An error occured determining the fwcc support: " << gluErrorString(error) << endl;
		*/
		}

		// Try doing a fixed-function operation.
		// Since this is a FCC, this should create an error.
	/*
		{
		cout << "Doing illegal operation (glTranslate) now..." << endl;
		glTranslatef(1,0,0);

		if (glGetError() != GL_NO_ERROR)
			cout << "Illegal operation caused an error message. :)" << endl;
		else
			cerr << "Illegal operation did NOT cause an error message. :(" << endl;
			
	}
	*/
}

void Context::setGLStates()
{
	glClearColor(0.442047, 0.387623, 0.361867, 1.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);

	glClearDepth(1.0f);

	glLineWidth(2.0f);
	glEnable(GL_LINE_SMOOTH);

	//glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FIXED_ONLY);
}

void Context::swapBuffers()
{
	glfwSwapBuffers();
	//glFrameTerminatorGREMEDY();
}

void Context::clearBuffers()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Context::toggleWireframe(void)
{
	wireframeMode = !wireframeMode;

	if(wireframeMode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		FontTool::getInstance().print("wireframe ON");
	}
	else 
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT, GL_FILL);
		FontTool::getInstance().print("wireframe OFF");
	}
}

