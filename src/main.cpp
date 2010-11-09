#include "common.hpp"
#include "shader.hpp"

#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>


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

	// Try doing a fixed-function operation.
	// Since this is a FCC, this should create an error.
	cout << "Doing illegal operation (glTranslate) now..." << endl;

	glGetError(); // Clear error state first..
	glTranslatef(1,0,0);

	if (glGetError() != GL_NO_ERROR)
	cout << "Illegal operation caused an error message. :)" << endl;
	else
	cerr << "Illegal operation did NOT cause an error message. :(" << endl;

}

void setGLOptions(void)
{
	glClearColor(0.442047, 0.387623, 0.361867, 1.0f);
	glEnable(GL_FRAMEBUFFER_SRGB);
}


void release_vbo_vao(GLuint *vbo_id, GLuint *vao_id)
{
	glDeleteVertexArrays(1, vao_id);

	glDeleteBuffers(2, vbo_id);

	*vao_id = 0;
	vbo_id[0] = vbo_id[1] = 0;
}


void init_vbo_vao(const Shader &shader, GLuint *vbo_id, GLuint *vao_id)
{
	// Our triangle data
	static GLfloat triangle_positions[] = {	 0.0f,		  1.0f, 0.0f, 1.0f,
		0.8660254f, -0.5f, 0.0f, 1.0f,
		-0.8660254f, -0.5f, 0.0f, 1.0f	};

	static GLfloat triangle_colors[] = {	1, 0, 0,
		0, 1, 0,
		0, 0, 1	};

	// Allocate and assign a Vertex Array Object to our handle

	glGenVertexArrays(1, vao_id);

	// Bind our Vertex Array Object as the current used object

	glBindVertexArray(*vao_id);

	// Allocate and assign two Vertex Buffer Objects (VBOs) to our handle
	glGenBuffers(2, vbo_id);

	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), triangle_positions, GL_STATIC_DRAW);

	GLint vertex_location = shader.get_attrib_location("in_position");
	glEnableVertexAttribArray(vertex_location);
	glVertexAttribPointer(	vertex_location, 4, GL_FLOAT,
		GL_FALSE, 0, NULL);

	// Repeat for second VBO storing vertex colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), triangle_colors, GL_STATIC_DRAW);

	GLint color_location = shader.get_attrib_location("in_color");
	glEnableVertexAttribArray(color_location);
	glVertexAttribPointer(	color_location, 3, GL_FLOAT,
		GL_FALSE, 0, NULL);

	// unbind VAO

	glBindVertexArray(0);

	// Unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


// This draws a shaded 2D triangle on the screen.
void draw (const Shader &shader, GLuint vao_id)
{
	glm::mat4 rotation_matrix =
		glm::gtx::transform::rotate(10.0f*(GLfloat)glfwGetTime(),
		0.0f, 0.0f, 1.0f);

	shader.bind();

	glBindVertexArray(vao_id);

	// Set our matrix uniform
	GLint matrix_location = shader.get_uniform_location("rotation_matrix");
	glUniformMatrix4fv( matrix_location,
		1,
		GL_FALSE,
		glm::value_ptr(rotation_matrix));


	// Draw triangle in VAO

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);

	// Unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader.unbind();
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

	GLuint vbo_id[2], vao_id;

	// Load and compile Shader files
	Shader minimal("../shader/minimal");

	if (!minimal) {
		cerr << "Could not compile minimal shader program." << endl;
		return 1;
	}

	minimal.bind_frag_data_location("out_color");

	init_vbo_vao(minimal, vbo_id, &vao_id);


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

	cout << "returning to earth..." << endl;
}



