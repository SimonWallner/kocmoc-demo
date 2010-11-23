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
	delete base;
}

bool Kocmoc::isRunning(){
	return running;
} 

void Kocmoc::start()
{
	while (running)
	{
		timer.tic();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw(*base, vao_id);

		Context::getInstance().swapBuffers();

		// Get OGL errors
		get_errors();

		// Check if the window has been closed
		running = running && glfwGetWindowParam( GLFW_OPENED );

		pollKeyboard();
		pollMouse();
		camera->updateMatrixes();
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
	base = new Shader("base.vert", "base.frag");

	if (!base) {
		cerr << "Could not compile base shader program." << endl;
		throw Exception("failed to compile base shader");
	}

	base->bind_frag_data_location("fragmentColor");

	// load and bind texture
	GLint texture = ImageLoader::getInstance().loadImage("color.png");

	GLint sTex0_location = base->get_uniform_location("sTex0");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(sTex0_location, 0);

	init_vbo_vao(*base, vbo_id, &vao_id);

	camera = new FilmCamera(vec3(0, 0, 10.0f), //eye
		vec3(0, 0, 0), // target
		vec3(0, 1, 0)); // up

	camera->updateMatrixes();
	
	running = true;

	//glfwDisable(GLFW_MOUSE_CURSOR);
}

void Kocmoc::draw(const Shader &shader, GLuint vao_id){

	glm::mat4 rotation_matrix =
		glm::gtx::transform::rotate(10.0f*(GLfloat)glfwGetTime(),
		0.0f, 0.0f, 1.0f);

	shader.bind();

	glBindVertexArray(vao_id);

	
	// Set uniforms
	GLint projectionMatrix_location = shader.get_uniform_location("projectionMatrix");
	glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	GLint viewMatrix_location = shader.get_uniform_location("viewMatrix");
	glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	GLint modelMatrix_location = shader.get_uniform_location("modelMatrix");
	glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, glm::value_ptr(mat4(1.0f)));

	//GLint lightPosition_location = shader.get_uniform_location("inLightPosition");
	//glUniform3fv(lightPosition_location, 3, glm::value_ptr(vec3(1.0f)));

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
	static GLfloat triangle_positions[] = {0.0f, 1.0f, 0.0f, 1.0f,
		0.8660254f, -0.5f, -1.0f, 1.0f,
		-0.8660254f, -0.5f, 1.0f, 1.0f};

	static GLfloat triangle_texCoord0[] = {1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 0.0f};
	
	static GLfloat triangle_normals[] = {0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f};

	// Allocate and assign a Vertex Array Object to our handle

	glGenVertexArrays(1, vao_id);

	// Bind our Vertex Array Object as the current used object

	glBindVertexArray(*vao_id);

	// Allocate and assign two Vertex Buffer Objects (VBOs) to our handle
	glGenBuffers(2, vbo_id);

	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 4 * sizeof(GLfloat), triangle_positions, GL_STATIC_DRAW);

	GLint vertex_location = shader.get_attrib_location("inPosition");
	glEnableVertexAttribArray(vertex_location);
	glVertexAttribPointer(vertex_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	// Repeat for second VBO storing tex coords
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(GLfloat), triangle_texCoord0, GL_STATIC_DRAW);

	GLint texCoord0_location = shader.get_attrib_location("inTexCoord0");
	glEnableVertexAttribArray(texCoord0_location);
	glVertexAttribPointer(texCoord0_location, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	// normals
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), triangle_normals, GL_STATIC_DRAW);

	GLint normal_location = shader.get_attrib_location("inNormal");
	glEnableVertexAttribArray(normal_location);
	glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);

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

	if (glfwGetKey('F'))
		timer.print();

	if (glfwGetKey(GLFW_KEY_UP))
		camera->dolly(vec3(0, 0, -0.01f));

	if (glfwGetKey(GLFW_KEY_DOWN))
		camera->dolly(vec3(0, 0, 0.01f));
	
	if (glfwGetKey(GLFW_KEY_LEFT))
		camera->dolly(vec3(-0.001f, 0, 0.f));
	
	if (glfwGetKey(GLFW_KEY_RIGHT))
		camera->dolly(vec3(0.001f, 0, 0.0f));
}


void Kocmoc::pollMouse()
{
	int newX, newY;
	glfwGetMousePos(&newX, &newY);

	camera->tumble((newY - mouseOldY)*0.001f, (newX - mouseOldX)*0.001f);

	mouseOldX = newX;
	mouseOldY = newY;
}