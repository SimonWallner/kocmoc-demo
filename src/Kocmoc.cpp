#include "Kocmoc.hpp"
#include "Context.hpp"
#include "Exception.hpp"
#include "Symbol.hpp"

#include <renderer/ShaderManager.hpp>
#include <renderer/FrameBuffer.hpp>
#include <renderer/ShadowMap.hpp>
#include <renderer/RenderMesh.hpp>
#include <loader/ImageLoader.hpp>
#include <loader/SceneLoader.hpp>
#include <util/util.hpp>
#include <util/Property.hpp>
#include <scene/SceneNode.hpp>
#include <scene/RenderMeshNode.hpp>
#include <scene/Octree.hpp>
#include <scene/OctreeNode.hpp>
#include <input/Gamepad.hpp>
#include <camera/FilmCamera.hpp>
#include <time/Clock.hpp>

#include <gtx/spline.hpp>

using namespace kocmoc;

using kocmoc::renderer::ShaderManager;
using kocmoc::renderer::ShadowMap;
using kocmoc::renderer::FrameBuffer;
using kocmoc::renderer::RenderMesh;
using kocmoc::loader::ImageLoader;
using kocmoc::loader::SceneLoader;
using kocmoc::camera::FilmCamera;
using kocmoc::scene::RenderMeshNode;
using kocmoc::scene::SceneNode;
using kocmoc::scene::Octree;
using kocmoc::scene::OctreeNode;
using kocmoc::input::Gamepad;
using kocmoc::camera::OrthoCamera;
using kocmoc::time::Clock;
using kocmoc::util::Property;
using kocmoc::util::generator::generateTriangle;
using kocmoc::util::generator::generateStars;

using glm::vec3;
using glm::mat4;

Kocmoc* Kocmoc::instance = NULL;
Kocmoc::ParamMapBool Kocmoc::paramMapBool;

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

	paramMapBool[symbolize("renderOctree")] = true;
	paramMapBool[symbolize("debugDrawMeshGizmo")] = Property("debugDrawMeshGizmo");
	paramMapBool[symbolize("debugDrawOctreeGizmo")] = Property("debugDrawOctreeGizmo");
	paramMapBool[symbolize("viewFrustumCulling")] = true;

	mouseSpeed = Property("mouseSpeed");
	keyboardSpeed = Property("keyboardSpeed");

	azimuthAngle = 0;
	elevationAngle = 2;
	updateSunDirection();
}

Kocmoc::~Kocmoc()
{
	delete gamepad;
}

bool Kocmoc::isRunning(){
	return running;
} 

void Kocmoc::stop(){
	running = false;
	std::cout << "shut down in progress" << std::endl;
}



void Kocmoc::init()
{
	{ // user camera
		camera = new FilmCamera(vec3(-15.0f, 40.0f, 0.0f), //eye
			vec3(0, 40.0f, 0), // target
			vec3(0, 1, 0));  // up
		float aspectRatio = (float)util::Property("aspectRatio") / ((float)Context::getInstance().width / (float)Context::getInstance().height);
		if (aspectRatio > 1) // horizontal letter box
			camera->setGateInPixel(Context::getInstance().width, (float)Context::getInstance().height / aspectRatio);
		else
			camera->setGateInPixel((float)Context::getInstance().width * aspectRatio, Context::getInstance().height);

		camera->setFocalLength(util::Property("cameraFocalLength35"));
		camera->setFilterMarginInPixel(util::Property("horizontalMargin"), util::Property("verticalMargin"));
		camera->updateMatrixes();

		// ortho cam
		orthoCamera = new OrthoCamera(vec3(0, 0, 0), vec3(-1, -1, -1), vec3(0, 1, 0));
		orthoCamera->updateMatrixes();
		shadowShader = ShaderManager::getInstance().load("shadow.vert", "shadow.frag");
	}




	// non octree scene node
	rootNodeNormal = new SceneNode("root node normal");
	rootNodeNormal->add(SceneLoader::getInstance().load(Property("modelName")));
	//rootNodeNormal->add(SceneLoader::getInstance().load("sky.dae"));


	// octree scene stuff
	//Octree* octree = new Octree(vec3(0), 2000);
	//
	//double time = glfwGetTime();
	//RenderMeshNode* kocmoc = SceneLoader::getInstance().load(Property("modelName"));
	//const RenderMeshNode::RenderMeshPointerList list = kocmoc->getRenderMeshList();
	//for (RenderMeshNode::RenderMeshPointerList::const_iterator ci = list.begin();
	//	ci != list.end();
	//	ci++)
	//{
	//	octree->insert((*ci), Property("octreeDepth"));
	//}
	//std::cout << "finished with octree inserting" << std::endl;
	//double delta = glfwGetTime() - time;
	//std::cout << "inserting took " << delta << "second" << std::endl;
	//OctreeNode* octreeNode = new OctreeNode(octree);
	//rootNodeNormal->add(octreeNode);
	




	{ // inputs 
		gamepad = new input::Gamepad(camera);
		useGamepad = gamepad->init();
		useMouse = util::Property("enableMouse");
		if (useMouse && util::Property("captureMouse"))
			glfwDisable(GLFW_MOUSE_CURSOR);
	}

	{
		fbo = new FrameBuffer(camera->getFrameWidth(), camera->getFrameHeight(), camera->getGateWidth(), camera->getGateHeight());
		fbo->setupShader();
		shadowMap = new ShadowMap(util::Property("shadowMapWidth"), util::Property("shadowMapHeight"));
	}
	
	{
		clock = new Clock();
		if (util::Property("useFixedFrameRate"))
			clock->start(1.0/(float)util::Property("fixedFrameRate"));
		else
			clock->start();
	}

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



		// drawing stuff ---------------

		//// shadow map
		//glDisable(GL_FRAMEBUFFER_SRGB);
		//glBindFramebuffer(GL_FRAMEBUFFER, shadowMap->getFBOHandle());
		//glViewport(0, 0, shadowMap->width, shadowMap->height);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//rootNode->draw(orthoCamera, shadowShader);
		
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->getFBOHandle());
		glViewport(0, 0, fbo->frameWidth, fbo->frameHeight);

		// TODO: move up/out of loop
		GLenum buffers[2] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, buffers);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, shadowMap->getTextureHandle());
		draw();


		glEnable(GL_FRAMEBUFFER_SRGB);
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

		Context::getInstance().swapBuffers();

		if (util::Property("recordSequence"))
			ImageLoader::getInstance().screenShot(true);
	}
}

void Kocmoc::draw()
{
	rootNodeNormal->draw(mat4(1), camera);
}

void Kocmoc::drawOverlays()
{
	// nothing to draw...
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


	if (glfwGetKey('W'))
		camera->dolly(vec3(0, 0, 4.0f) * keyboardSpeed * (float)clock->lastFrameDuration());

	if (glfwGetKey('S'))
		camera->dolly(vec3(0, 0, -4.0f) * keyboardSpeed * (float)clock->lastFrameDuration());
	
	if (glfwGetKey('A'))
		camera->dolly(vec3(-4.0f, 0, 0.0f) * keyboardSpeed * (float)clock->lastFrameDuration());
	
	if (glfwGetKey('D'))
		camera->dolly(vec3(4.0f, 0, 0.0f) * keyboardSpeed * (float)clock->lastFrameDuration());

	if (glfwGetKey('1'))
	{
		paramMapBool[symbolize("renderOctree")] = !paramMapBool[symbolize("renderOctree")];
		std::cout << "octree rendering is: " << paramMapBool[symbolize("renderOctree")] << std::endl;
	}

	if (glfwGetKey('2'))
	{
		paramMapBool[symbolize("debugDrawMeshGizmo")] = !paramMapBool[symbolize("debugDrawMeshGizmo")];
		std::cout << "drawing debug mesh gizmos is: " << paramMapBool[symbolize("debugDrawMeshGizmo")] << std::endl;
	}

	if (glfwGetKey('3'))
	{
		paramMapBool[symbolize("debugDrawOctreeGizmo")] = !paramMapBool[symbolize("debugDrawOctreeGizmo")];
		std::cout << "drawing debug octree gizmos is: " << paramMapBool[symbolize("debugDrawOctreeGizmo")] << std::endl;
	}

	if (glfwGetKey('4'))
	{
		paramMapBool[symbolize("viewFrustumCulling")] = !paramMapBool[symbolize("viewFrustumCulling")];
		std::cout << "fiew frustum culling is: " << paramMapBool[symbolize("viewFrustumCulling")] << std::endl;
	}


	// sun control
	if (glfwGetKey(GLFW_KEY_UP))
	{
		elevationAngle += 90.0f * (float)clock->lastFrameDuration();
		std::cout << "elevation angle: " << elevationAngle << std::endl;
		updateSunDirection();
	}
	if (glfwGetKey(GLFW_KEY_DOWN))
	{
		elevationAngle -= 090.0f * (float)clock->lastFrameDuration();
		std::cout << "elevation angle: " << elevationAngle << std::endl;
		updateSunDirection();
	}
	if (glfwGetKey(GLFW_KEY_LEFT))
	{
		azimuthAngle += 90.0f * (float)clock->lastFrameDuration();
		std::cout << "azimuth angle: " << azimuthAngle << std::endl;
		updateSunDirection();
	}
	if (glfwGetKey(GLFW_KEY_RIGHT))
	{
		azimuthAngle -= 90.0f * (float)clock->lastFrameDuration();
		std::cout << "azimuth angle: " << azimuthAngle << std::endl;
		updateSunDirection();
	}
}

void Kocmoc::updateSunDirection()
{
	mat4 elevation = glm::rotate(elevationAngle, vec3(0, 0, 1));
	mat4 rotation = glm::rotate(azimuthAngle, vec3(0, 1, 0));


	sunDirection = vec3(rotation * elevation * glm::vec4(1, 0, 0, 1));

}

void Kocmoc::pollMouse()
{
	int newX, newY;
	glfwGetMousePos(&newX, &newY);

	camera->tumble((newX - mouseOldX) * (float)clock->lastFrameDuration() * 0.1f * mouseSpeed 
		, (newY - mouseOldY) * -(float)clock->lastFrameDuration() * 0.1f * mouseSpeed);

	mouseOldX = newX;
	mouseOldY = newY;
}


void Kocmoc::reload()
{
	fbo->setupShader();
}

Clock* Kocmoc::getClock()
{
	return clock;
}