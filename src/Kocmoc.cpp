#include "Kocmoc.hpp"
#include "Context.hpp"
#include "Property.hpp"
#include "Exception.hpp"
#include "ImageLoader.hpp"
#include "KocmocLoader.hpp"
#include "utility.hpp"
#include "ShaderManager.hpp"
#include "audioPlayer.hpp"
#include "AnimationSystem.hpp"

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
	showGizmos = util::Property("debugShowGizmo");
}

Kocmoc::~Kocmoc()
{
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
	camera = new FilmCamera(vec3(0.0f, 0.0f, 15.0f), //eye
		vec3(0, 0, 0), // target
		vec3(0, 1, 0));  // up
	float aspectRatio = (float)util::Property("aspectRatio") / ((float)Context::getInstance().width / (float)Context::getInstance().height);
	if (aspectRatio > 1) // horizontal letter box
		camera->setGateInPixel(Context::getInstance().width, (float)Context::getInstance().height / aspectRatio);
	else
		camera->setGateInPixel((float)Context::getInstance().width * aspectRatio, Context::getInstance().height);

	camera->setFocalLength(util::Property("cameraFocalLength35"));
	camera->setFilterMarginInPixel(util::Property("horizontalMargin"), util::Property("verticalMargin"));
	camera->setupGizmo();
	camera->updateMatrixes();

	orthoCam = new OrthoCam(vec3(0, 0, 0), vec3(-1, -1, -1), vec3(0, 1, 0));
	orthoCam->updateMatrixes();
	
	
	scene = new KocmocScene();
	ship = KocmocLoader::getInstance().load(util::Property("XModelName"));
	scene->add(ship);
	scene->add(util::generator::generateStars());
	shadowShader = ShaderManager::getInstance().load("shadow.vert", "shadow.frag");

	if (showGizmos)
		scene->add(util::generator::generateGizmo());


	{ /* inputs */
		gamepad = new input::Gamepad(camera);
		useGamepad = gamepad->init();
		useMouse = util::Property("enableMouse");
		if (useMouse && util::Property("captureMouse"))
			glfwDisable(GLFW_MOUSE_CURSOR);
	}

	fbo = new FrameBuffer(camera->getFrameWidth(), camera->getFrameHeight(), camera->getGateWidth(), camera->getGateHeight());
	fbo->setupShader();
	shadowMap = new ShadowMap(util::Property("shadowMapWidth"), util::Property("shadowMapHeight"));
	
	AudioPlayer::getInstance().play("music_mono_low.ogg");

	clock = new Clock();
	if (util::Property("useFixedFrameRate"))
		clock->start(1.0/24);
	else
		clock->start();

	animationClock = new AnimationClock(clock);
	AnimationSystem::getInstance().parseAnimationFile();

	overlayCam = new OverlayCam(Context::getInstance().width, Context::getInstance().height);
	overlayCam->updateMatrixes();
	black = new ImageOverlay("black.png", Context::getInstance().width, Context::getInstance().height);
	title = new ImageOverlay("title.png", 640, 480);


	running = true;
}

void Kocmoc::start()
{
	while (running)
	{
		clock->awaitSchedule();
		clock->tick();
		timer.tic();


		// Check if the window has been closed
		running = running && glfwGetWindowParam( GLFW_OPENED );

		pollKeyboard();
		if (useGamepad)
			gamepad->poll();
		
		if (useMouse)
			pollMouse();

		camera->updateMatrixes();

		// update stuff ------------------
		ship->setTransformation(glm::gtx::transform::rotate(10.0f*(GLfloat)animationClock->getTime(), 1.0f, 0.0f, 0.0f));
		black->setAlpha(AnimationSystem::getInstance().getScalar(animationClock->getTime(), "black_alpha"));
		title->setAlpha(AnimationSystem::getInstance().getScalar(animationClock->getTime(), "title_alpha"));


		// drawing stuff ---------------

		// shadow map
		glDisable(GL_FRAMEBUFFER_SRGB);
		glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->getFBOHandle());
		glViewport(0, 0, shadowMap->width, shadowMap->height);
		glClear(GL_DEPTH_BUFFER_BIT);
		ship->draw(orthoCam, shadowShader);
		
		glEnable(GL_FRAMEBUFFER_SRGB);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->getFBOHandle());
		glViewport(0, 0, fbo->frameWidth, fbo->frameHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, shadowMap->getTextureHandle());
		draw();


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, Context::getInstance().width, Context::getInstance().height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if (Context::getInstance().getWireframe())
		{
			Context::getInstance().setWireframe(false);
			fbo->drawFBO();
			Context::getInstance().setWireframe(true);
		}
		else
			fbo->drawFBO();

		drawOverlays();

		Context::getInstance().swapBuffers();
		if (util::Property("recordSequence"))
			ImageLoader::getInstance().screenShot(true);
	}
}

void Kocmoc::draw()
{
	scene->draw(camera);	
}

void Kocmoc::drawOverlays()
{
	//if (showGizmos)
	//	camera->drawGizmo();

	glDisable(GL_DEPTH_TEST);
	black->draw();
	title->draw();
	glEnable(GL_DEPTH_TEST);
}

void Kocmoc::pollKeyboard(void)
{
	running = running && !glfwGetKey( GLFW_KEY_ESC );

	if (glfwGetKey(GLFW_KEY_F1))
		std::cout	<< "---------------------------------------------------------" << std::endl
		<< "\t F1: this help dialog" <<std::endl
		<< "\t F3: toggle wireframe" << std::endl
		<< "\t F4: print frametime/fps" << std::endl
		<< "\t F5: toggle non-planar projection post" << std::endl
		<< "\t F6: toggle vignetting post" << std::endl
		<< "\t F7: toggle color correction post" << std::endl
		<< "\t '.': take screenshot" << std::endl
		<< "\t R: reload shaders/textures/etc..." << std::endl;

	if (glfwGetKey(GLFW_KEY_F3))
		Context::getInstance().setWireframe(!Context::getInstance().getWireframe());

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
		camera->dolly(vec3(0, 0, 4.0f) * (float)clock->lastFrameDuration());

	if (glfwGetKey('S'))
		camera->dolly(vec3(0, 0, -4.0f) * (float)clock->lastFrameDuration());
	
	if (glfwGetKey('A'))
		camera->dolly(vec3(-4.0f, 0, 0.0f) * (float)clock->lastFrameDuration());
	
	if (glfwGetKey('D'))
		camera->dolly(vec3(4.0f, 0, 0.0f) * (float)clock->lastFrameDuration());

	if (glfwGetKey(GLFW_KEY_SPACE))
		camera->dolly(vec3(0.0f, 4.0f, 0.0f) * (float)clock->lastFrameDuration());

	if (glfwGetKey(GLFW_KEY_LSHIFT))
		camera->dolly(vec3(0.0f, -4.0f, 0.0f) * (float)clock->lastFrameDuration());
}


void Kocmoc::pollMouse()
{
	int newX, newY;
	glfwGetMousePos(&newX, &newY);

	camera->tumble((newX - mouseOldX) * (float)clock->lastFrameDuration() * 0.1f
		, (newY - mouseOldY) * -(float)clock->lastFrameDuration() * 0.1f);

	mouseOldX = newX;
	mouseOldY = newY;
}


void Kocmoc::reload()
{
	fbo->setupShader();
}