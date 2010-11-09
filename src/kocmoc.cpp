#include "Context.hpp"
#include "ConfigFileParser.hpp"


Kocmoc* Kocmoc::instance = NULL;

Kocmoc& Kocmoc::getInstance(void)
{
	if(!instance)
		instance = new Kocmoc();
	return *instance;
}

void Kocmoc::Destroy()
{
	// static
	delete instance;
	instance = NULL;
}

Kocmoc::Kocmoc()
{
}

Kocmoc::~Kocmoc()
{
}

bool Kocmoc::isRunning(){
	return running;
} 

void Kocmoc::start()
{
	while (running)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			draw(minimal, vao_id);

			Context::getInstance().swapBuffers();

			// Get OGL errors

			get_errors();

			// Check if the window has been closed

			running = running && !glfwGetKey( GLFW_KEY_ESC );
			running = running && !glfwGetKey( 'Q' );
			running = running && glfwGetWindowParam( GLFW_OPENED );
		}

		// Get OGL errors
		get_errors();

	}
	
	// wait for the input thread to finish
	glfwWaitThread(inputLoop, GLFW_WAIT);
}

void Kocmoc::stop(){
	running = false;
}


void Kocmoc::init()
{
	GLuint vbo_id[2], vao_id;

	// Load and compile Shader files
	Shader minimal("minimal.vert", "minimal.frag");

	if (!minimal) {
		cerr << "Could not compile minimal shader program." << endl;
		return 1;
	}

	minimal.bind_frag_data_location("out_color");

	init_vbo_vao(minimal, vbo_id, &vao_id);
	
	running = true;
}

void Kocmoc::draw(){

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
