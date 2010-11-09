#include <iostream>
#include <GL/glfw.h>

using namespace std;

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

	bool running = true;
	while (running)
	{
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		draw(minimal, vao_id);
//
//		glfwSwapBuffers();
//
//		// Get OGL errors
//
//		get_errors();

		// Check if the window has been closed

		running = running && !glfwGetKey( GLFW_KEY_ESC );
		running = running && !glfwGetKey( 'Q' );
		running = running && glfwGetWindowParam( GLFW_OPENED );
	}


}
