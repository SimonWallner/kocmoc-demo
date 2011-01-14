#include "Gamepad.hpp"

#include "common.hpp"
#include <math.h>
#include "Kocmoc.hpp"
#include "ShaderManager.hpp"

using namespace kocmoc;
using namespace kocmoc::input;

Gamepad::Gamepad(FilmCamera* _camera) :
	camera(_camera)
{
	deadZone = 0.20f;
}

Gamepad::~Gamepad()
{
}

bool Gamepad::init(void)
{
	enableGamepad = false;
	
	cout << "detecting joysticks..." << endl;
	for (int i = GLFW_JOYSTICK_1; i <= GLFW_JOYSTICK_LAST; i++)
	{
		if (glfwGetJoystickParam(i, GLFW_PRESENT) == GL_TRUE)
		{
			numJoystick = i;
			numButtons = glfwGetJoystickParam(i, GLFW_BUTTONS);
			numAxes = glfwGetJoystickParam(i, GLFW_AXES);
			enableGamepad = true;
			break;
		}
	}
	if (enableGamepad)
	{
		cout << "found joystick no: " << numJoystick;
		cout << " with " << numButtons << " buttons and ";
		cout << numAxes << " axes" << endl;
	} 
	else
		cout << "no joystick(gamepad) found" << endl;

	return enableGamepad;
}

void Gamepad::poll(void)
{
	if (enableGamepad) {
		GLfloat *pos = new GLfloat[numAxes];
		int activeAxes = glfwGetJoystickPos(numJoystick, pos, numAxes);

		/*
		* 360 gamepad assignment: (direction given for positive values)
		* - A0: left stick right
		* - A1: left stick up
		* - A2: left trigger (right trigger is [-1, 0]
		* - A3: right stick down(!)
		* - A4: right stick right
		*/
		if (abs(pos[0]) > deadZone) // left stick right
			camera->dolly(vec3(pos[0] * 0.01f, 0, 0));

		if (abs(pos[1]) > deadZone) // left stick up
			camera->dolly(vec3(0, 0, pos[1] * 0.01f));

		if (abs(pos[3]) > deadZone) // right stick down
			camera->tumble(0, -pos[3] * 0.005f);

		if (abs(pos[4]) > deadZone) // right stick right
			camera->tumble(pos[4] * 0.005f, 0);

		if (abs(pos[2]) > deadZone) // trigger
			camera->dolly(vec3(0, -pos[2] * 0.01f, 0));

		unsigned char *buttons = new unsigned char[numButtons];
		int activeButtons = glfwGetJoystickButtons(numJoystick, buttons, numButtons);

		/*
		* Button assignment 360 gamepad
		* B0: A
		* B1: B
		* B2: X
		* B3: Y
		* B4: left bumper
		* B5: right bumper
		* B6: back
		* B7: start
		* B8: left stick
		* B9: right stick
		*/

		if (buttons[6] == GLFW_PRESS) // quit app (back button)
			Kocmoc::getInstance().stop();

		if (buttons[7] == GLFW_PRESS) // start button, reload
			ShaderManager::getInstance().reloadAll();

		if (buttons[4] == GLFW_PRESS) // left bumber, turn ccw
			camera->rotate(-0.2f);

		if (buttons[5] == GLFW_PRESS) // right bumber, turn cw
			camera->rotate(0.2f);
		
		delete pos;
		delete buttons;
	}
}
