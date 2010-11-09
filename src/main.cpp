#include <iostream>
#include <GL/glfw.h>

using namespace std;

// Test if we got a valid forward compatible context (FCC)
void test_ogl3(void)
{
	GLint profile;

	// retrieve our OpenGL-version
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// check if we have a core-profile
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
	if (profile == GL_CONTEXT_CORE_PROFILE_BIT) {
		std::cout << "Having a core-profile" << std::endl;
	} else {
		std::cout << "Having a compatibility-profile" << std::endl;
	}

	//gl3w doesn't expose deprecated functions anymore!
	/*
	// Try doing a fixed-function operation.
	// Since this is a FCC, this should create an error.
	cout << "Doing illegal operation (glTranslate) now..." << endl;

	glGetError(); // Clear error state first..
	glTranslatef(1,0,0);

	if (glGetError() != GL_NO_ERROR)
	cout << "Illegal operation caused an error message. :)" << endl;
	else
	cerr << "Illegal operation did NOT cause an error message. :(" << endl;
	*/
}

void setGLOptions(void)
{
	glClearColor(0.69f, 0.65f, 0.63f, 1.0f);
}

int main(void)
{
	cout << "starting KOCMOC, please stand by" << endl;

	int width=800,
		height=800;

	glfwInit();


	// Set flags so GLFW creates the desired OpenGL context
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if (glfwOpenWindow(	width,height,
		0,0,0,0,
		24, 8,
		GLFW_WINDOW) != GL_TRUE) {
			cerr << "Failed to initialize OpenGL window." << endl;
			glfwTerminate();
			return 1;
	}

	glfwSetWindowTitle("KOCMOC");

	test_ogl3();
	setGLOptions();

	bool running = true;
	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//		draw(minimal, vao_id);

		glfwSwapBuffers();

		// Get OGL errors

//		get_errors();

		// Check if the window has been closed

		running = running && !glfwGetKey( GLFW_KEY_ESC );
		running = running && !glfwGetKey( 'Q' );
		running = running && glfwGetWindowParam( GLFW_OPENED );
	}
}



