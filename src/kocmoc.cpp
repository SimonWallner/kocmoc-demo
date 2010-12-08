#include "kocmoc.hpp"

#include "Context.hpp"
#include "PropertiesFileParser.hpp"
#include "Exception.hpp"
#include "ImageLoader.hpp"
#include "KocmocLoader.hpp"

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
	glfwGetMousePos(&mouseOldX, &mouseOldY);
}

Kocmoc::~Kocmoc()
{
	delete base;
	delete scene;
}

bool Kocmoc::isRunning(){
	return running;
} 

void Kocmoc::stop(){
	running = false;
}



void Kocmoc::init()
{
	// Load and compile Shader files
	base = new Shader("base.vert", "base.frag");

	if (!base) {
		cerr << "Could not compile base shader program." << endl;
		throw Exception("failed to compile base shader");
	}

	base->bind();

	// load and bind texture
	GLint texture = ImageLoader::getInstance().loadImage("color.png");
	GLint sTex0_location = base->get_uniform_location("sTex0");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	//base->bind();
	glUniform1i(sTex0_location, 0);

	get_errors();

	camera = new FilmCamera(vec3(0, 0, 3.0f), //eye
		vec3(0, 0, 0), // target
		vec3(0, 1, 0)); // up
	camera->updateMatrixes();
	
	if (KOCMOC_DEBUG_CAPTURE_MOUSE)
		glfwDisable(GLFW_MOUSE_CURSOR);

	
	scene = KocmocLoader::getInstance().load("suzanne-hires.dae");
	//scene = new KocmocScene();

	PolyMesh *poly = new PolyMesh(16);
	static GLfloat positions[] = {
			0.5f, 0.5f, 0.5f, // front 
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			
			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, -0.5f, // back 
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,

			-0.5f, 0.5f, 0.5f, // left 
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f};

	static GLfloat uv0[] = {
			0.5f, 0.5f,
			-0.5f, 0.5f,
			-0.5f, -0.5f,

			0.5f, 0.5f,
			-0.5f, 0.5f,
			-0.5f, -0.5f,

			0.5f, 0.5f,
			-0.5f, 0.5f,
			-0.5f, -0.5f,

			0.5f, 0.5f,
			-0.5f, 0.5f,
			-0.5f, -0.5f,

			0.5f, 0.5f,
			-0.5f, 0.5f,
			-0.5f, -0.5f,
			
			0.5f, -0.5f,
			-0.5f, 0.5f,
			-0.5f, -0.5f};

	poly->setVertexPositions(positions);
	poly->setUV0(uv0);

	//scene->addPolyMesh(poly);
	scene->transferData(base);
	
	running = true;
}

void Kocmoc::start()
{
	while (running)
	{
		timer.tic();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		draw();

		Context::getInstance().swapBuffers();

		if (_DEBUG)
			get_errors();

		// Check if the window has been closed
		running = running && glfwGetWindowParam( GLFW_OPENED );

		pollKeyboard();
		pollMouse();
		camera->updateMatrixes();
	}

	get_errors();
}

void Kocmoc::draw()
{
	glm::mat4 rotation_matrix =		glm::gtx::transform::rotate(10.0f*(GLfloat)glfwGetTime(),		0.0f, 0.0f, 1.0f);
	GLint projectionMatrix_location = base->get_uniform_location("projectionMatrix");	glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
	GLint viewMatrix_location = base->get_uniform_location("viewMatrix");	glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
	GLint modelMatrix_location = base->get_uniform_location("modelMatrix");	glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, glm::value_ptr(rotation_matrix));
	

	scene->draw();
}


void Kocmoc::pollKeyboard(void)
{
	running = running && !glfwGetKey( GLFW_KEY_ESC );
	running = running && !glfwGetKey( 'Q' );

	if (glfwGetKey('1'))
		Context::getInstance().toggleWireframe();

	if (glfwGetKey('.'))
		ImageLoader::getInstance().screenShot();

	if (glfwGetKey('2'))
		timer.print();

	if (glfwGetKey('W'))
		camera->dolly(vec3(0, 0, -0.01f));

	if (glfwGetKey('S'))
		camera->dolly(vec3(0, 0, 0.01f));
	
	if (glfwGetKey('A'))
		camera->dolly(vec3(-0.01f, 0, 0.f));
	
	if (glfwGetKey('D'))
		camera->dolly(vec3(0.01f, 0, 0.0f));

	if (glfwGetKey(GLFW_KEY_SPACE))
		camera->dolly(vec3(0.0f, 0.01f, 0.0f));

	if (glfwGetKey(GLFW_KEY_LSHIFT))
		camera->dolly(vec3(0.0f, -0.01f, 0.0f));
}


void Kocmoc::pollMouse()
{
	int newX, newY;
	glfwGetMousePos(&newX, &newY);

	camera->tumble((newY - mouseOldY)*0.01f, (newX - mouseOldX)*0.01f);

	mouseOldX = newX;
	mouseOldY = newY;
}
