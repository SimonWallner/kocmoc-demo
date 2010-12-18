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
	useFBO = true;
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
	
	
	scene = KocmocLoader::getInstance().load("suzanne-hires.dae");
	scene->transferData(base);

	// generate starts, lots of stars
	int starCount = 10000;
	float domain = 100;
	float size = 2;

	PolyMesh *stars = new PolyMesh(starCount * 3);
	GLfloat *positions = new GLfloat[starCount * 3 * 3];
	GLfloat *colors = new GLfloat[starCount * 3 * 3];
	
	for (int i = 0; i < starCount; i++)
	{
		vec3 v0 = vec3((rand() * domain)/RAND_MAX - (domain / 2), 
			(rand() * domain)/RAND_MAX - (domain / 2), 
			(rand() * domain)/RAND_MAX - (domain / 2));

		vec3 direction1 = vec3((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);
		vec3 direction2 = vec3((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX);

		vec3 v1 = v0 + direction1 * size;
		vec3 v2 = (v0 + direction1 * (size/ 2)) + direction2 * (0.86602540378443864f * size);
		
		positions[i*9] = v0.x;
		positions[i*9+1] = v0.y;
		positions[i*9+2] = v0.z;
		positions[i*9+3] = v1.x;
		positions[i*9+4] = v1.y;
		positions[i*9+5] = v1.z;
		positions[i*9+6] = v2.x;
		positions[i*9+7] = v2.y;
		positions[i*9+8] = v2.z;

		colors[i*9] = 1.0f;
		colors[i*9+1] = 1.0f;
		colors[i*9+2] = 1.0f;
		colors[i*9+3] = 1.0f;
		colors[i*9+4] = 1.0f;
		colors[i*9+5] = 1.0f;
		colors[i*9+6] = 1.0f;
		colors[i*9+7] = 1.0f;
		colors[i*9+8] = 1.0f;
	}
	stars->setVertexPositions(positions);
	stars->setVertexNormals(colors);
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
