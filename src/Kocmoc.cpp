#include "Kocmoc.hpp"
#include "Context.hpp"
#include "Property.hpp"
#include "Exception.hpp"
#include "ImageLoader.hpp"
#include "KocmocLoader.hpp"
#include "utility.hpp"
#include "ShaderManager.hpp"

using namespace kocmoc;

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
	useFBO = util::Property("debugEnableFBO");
	showGizmos = util::Property("debugShowGizmo");
}

Kocmoc::~Kocmoc()
{
	delete base;
	delete scene;
	delete gamepad;
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

	camera = new FilmCamera(vec3(0.0f, 0.0f, 15.0f), //eye
		vec3(0, 0, 0), // target
		vec3(0, 1, 0),  // up
		(float)Context::getInstance().width / (float)Context::getInstance().height); // aspect ration
	camera->setFocalLength(util::Property("cameraFocalLength35"));
	camera->setupGizmo();
	camera->updateMatrixes();
	
	
	scene = new KocmocScene();
	ship = KocmocLoader::getInstance().load(util::Property("XModelName"));
	scene->add(ship);
	scene->add(util::generator::generateStars());

	if (showGizmos)
		scene->add(util::generator::generateGizmo());


	{ /* inputs */
		gamepad = new input::Gamepad(camera);
		useGamepad = gamepad->init();
	}

	fbo = new FrameBuffer(Context::getInstance().width, Context::getInstance().height);
	fbo->setupShader();
	
	running = true;
}

void Kocmoc::start()
{
	while (running)
	{
		timer.tic();


		// Check if the window has been closed
		running = running && glfwGetWindowParam( GLFW_OPENED );

		pollKeyboard();
		if (useGamepad)
			gamepad->poll();
		else
			pollMouse();

		camera->updateMatrixes();

		// update stuff ------------------
		ship->setTransformation(glm::gtx::transform::rotate(10.0f*(GLfloat)glfwGetTime(), 1.0f, 0.0f, 0.0f));


		// drawing stuff ---------------
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (!useFBO)
			draw();
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo->getFBOHandle());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			draw();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			fbo->drawFBO();
		}
		drawOverlays();

		Context::getInstance().swapBuffers();

		
	}
}

void Kocmoc::draw()
{
	scene->draw();	
}
void Kocmoc::drawOverlays()
{
	if (showGizmos)
		camera->drawGizmo();
}

void Kocmoc::pollKeyboard(void)
{
	running = running && !glfwGetKey( GLFW_KEY_ESC );

	if (glfwGetKey(GLFW_KEY_F1))
		std::cout	<< "---------------------------------------------------------" << std::endl
		<< "\t F1: this help dialog" <<std::endl
		<< "\t F2: toggle FBO" << std::endl
		<< "\t F3: toggle wireframe" << std::endl
		<< "\t F4: print frametime/fps" << std::endl
		<< "\t F5: toggle non-planar projection post" << std::endl
		<< "\t F6: toggle vignetting post" << std::endl
		<< "\t F7: toggle color correction post" << std::endl
		<< "\t '.': take screenshot" << std::endl
		<< "\t R: reload shaders/textures/etc..." << std::endl;

	if (glfwGetKey(GLFW_KEY_F2))
		useFBO = !useFBO;

	if (glfwGetKey(GLFW_KEY_F3))
		Context::getInstance().toggleWireframe();

	if (glfwGetKey(GLFW_KEY_F4))
		timer.print();

	if (glfwGetKey(GLFW_KEY_F5))
	{
		fbo->toggleProjection();
		reload();
	}

	if (glfwGetKey(GLFW_KEY_F6))
	{
		fbo->toggleVignetting();
		reload();
	}

	if (glfwGetKey(GLFW_KEY_F7))
	{
		fbo->toggleColorCorrection();
		reload();
	}

	if (glfwGetKey('.'))
		ImageLoader::getInstance().screenShot();

	if (glfwGetKey('R'))
		ShaderManager::getInstance().reloadAll();


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

	camera->tumble((newX - mouseOldX)*0.01f, (newY - mouseOldY)*0.01f);

	mouseOldX = newX;
	mouseOldY = newY;
}


void Kocmoc::reload()
{
	fbo->setupShader();
}