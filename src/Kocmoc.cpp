#include "Kocmoc.hpp"
#include "Context.hpp"
#include "PropertiesFileParser.hpp"
#include "Exception.hpp"
#include "ImageLoader.hpp"
#include "KocmocLoader.hpp"

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
	PropertiesFileParser::GetInstance().getProperty("debugUseFBO", &useFBO);
	PropertiesFileParser::GetInstance().getProperty("debugShowGizmo", &showGizmos);
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

	get_errors();

	camera = new FilmCamera(vec3(0, 0, 3.0f), //eye
		vec3(0, 0, 0), // target
		vec3(0, 1, 0),  // up
		(float)Context::getInstance().width / (float)Context::getInstance().height); // aspect ration
	camera->setupGizmo();
	camera->updateMatrixes();
	
	
	//scene = KocmocLoader::getInstance().load("suzanne-hires.dae");
	scene = new KocmocScene();
	scene->transferData(base);

	// generate starts, lots of stars
	int starCount;
	float domain, size;
	PropertiesFileParser::GetInstance().getProperty("starsCount", &starCount);
	PropertiesFileParser::GetInstance().getProperty("starsDomain", &domain);
	PropertiesFileParser::GetInstance().getProperty("starsSize", &size);
	

	PolyMesh *stars = new PolyMesh(starCount * 4, starCount * 4 * 4, starCount * 4);

	double *positions = new double[stars->getVertexCount() * 3];
	unsigned int *vertexIndices = new unsigned int[stars->getVertexIndexCount()];
	unsigned int *firstIndices = new unsigned int[stars->getPrimitiveCount()];
		
	for (int i = 0; i < starCount; i++)
	{
		vec4 v0 = vec4(0, 0, 0, 1) * size;
		vec4 v1 = vec4(0, 1, 1, 1) * size;
		vec4 v2 = vec4(1, 1, 0, 1) * size;
		vec4 v3 = vec4(1, 0, 1, 1) * size;

		mat4 rotation = glm::gtx::transform::rotate(((float)rand() * 360)/RAND_MAX,
			(float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);

		mat4 transform = glm::translate((rand() * domain)/RAND_MAX - (domain / 2),
			(rand() * domain)/RAND_MAX - (domain / 2),
			(rand() * domain)/RAND_MAX - (domain / 2)) * rotation;

		v0 = transform * v0;
		v1 = transform * v1;
		v2 = transform * v2;
		v3 = transform * v3;

		// ad vertices
		positions[i*12] = v0.x;
		positions[i*12+1] = v0.y;
		positions[i*12+2] = v0.z;
		positions[i*12+3] = v1.x;
		positions[i*12+4] = v1.y;
		positions[i*12+5] = v1.z;
		positions[i*12+6] = v2.x;
		positions[i*12+7] = v2.y;
		positions[i*12+8] = v2.z;
		positions[i*12+9] = v3.x;
		positions[i*12+10] = v3.y;
		positions[i*12+11] = v3.z;
		

		// add primitives and indices, 
		// 0 - 1 - 2
		firstIndices[i * 4] = i*12;
		vertexIndices[i*12] = i*4;
		vertexIndices[i*12+1] = i*4 +1;
		vertexIndices[i*12+2] = i*4 +2;
		
		// 1 - 2 - 3
		firstIndices[i*4+1] = i*12+3;
		vertexIndices[i*12+3] = i*4 +1;
		vertexIndices[i*12+4] = i*4 +2;
		vertexIndices[i*12+5] = i*4 +3;

		// 3 - 0 - 1
		firstIndices[i*4+2] = i*12+6;
		vertexIndices[i*12+6] = i*4 +3;
		vertexIndices[i*12+7] = i*4 +0;
		vertexIndices[i*12+8] = i*4 +1;

		// 0 - 2 - 3
		firstIndices[i*4+3] = i*12+9;
		vertexIndices[i*12+9] = i*4 +0;
		vertexIndices[i*12+10] = i*4 +2;
		vertexIndices[i*12+11] = i*4 +3;
	}

	stars->setFirstIndexArray(firstIndices);
	stars->setVertexIndexArray(vertexIndices);
	stars->setVertexPositions(positions);

	// add shader to poly
	Shader *shader = new Shader("base.vert", "base.frag");
	stars->setShader(shader);

	// add texture
	GLuint tex = ImageLoader::getInstance().loadImage("color.png");
	stars->setTexture(tex);

	scene->addPolyMesh(stars);



	{ /* inputs */
		gamepad = new Gamepad(camera);
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

		if (_DEBUG)
			get_errors();

		// Check if the window has been closed
		running = running && glfwGetWindowParam( GLFW_OPENED );

		pollKeyboard();
		if (useGamepad)
			gamepad->poll();
		else
			pollMouse();

		camera->updateMatrixes();
	}

	get_errors();
}

void Kocmoc::draw()
{
	scene->draw(base);	
}
void Kocmoc::drawOverlays()
{
	if (showGizmos)
		camera->drawGizmo();
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

	if (glfwGetKey('3'))
		useFBO = !useFBO;

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
