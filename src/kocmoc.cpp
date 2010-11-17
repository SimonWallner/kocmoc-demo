#include "kocmoc.hpp"

#include "Context.hpp"
#include "PropertiesFileParser.hpp"
#include "Exception.hpp"
#include "ImageLoader.hpp"

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
	delete minimal;
}

bool Kocmoc::isRunning(){
	return running;
} 

void Kocmoc::start()
{
	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw(*minimal, vao_id);

		Context::getInstance().swapBuffers();

		// Get OGL errors
		get_errors();

		// Check if the window has been closed
		running = running && glfwGetWindowParam( GLFW_OPENED );

		pollKeyboard();
	}

	// Get OGL errors
	get_errors();
}


void Kocmoc::stop(){
	running = false;
}


void Kocmoc::init()
{
	GLuint vbo_id[2];

	// Load and compile Shader files
	minimal = new Shader("minimal.vert", "minimal.frag");

	if (!minimal) {
		cerr << "Could not compile minimal shader program." << endl;
		throw Exception("failed to compile minimal shader");
	}

	minimal->bind_frag_data_location("out_color");

	init_vbo_vao(*minimal, vbo_id, &vao_id);
	
	running = true;
}

void Kocmoc::draw(const Shader &shader, GLuint vao_id){

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

void Kocmoc::init_vbo_vao(const Shader &shader, GLuint *vbo_id, GLuint *vao_id)
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

void Kocmoc::release_vbo_vao(GLuint *vbo_id, GLuint *vao_id)
{
	glDeleteVertexArrays(1, vao_id);

	glDeleteBuffers(2, vbo_id);

	*vao_id = 0;
	vbo_id[0] = vbo_id[1] = 0;
}

void Kocmoc::pollKeyboard(void)
{
	running = running && !glfwGetKey( GLFW_KEY_ESC );
	running = running && !glfwGetKey( 'Q' );

	if (glfwGetKey('W'))
		Context::getInstance().toggleWireframe();

	if (glfwGetKey('S'))
		ImageLoader::getInstance().screenShot();
}
