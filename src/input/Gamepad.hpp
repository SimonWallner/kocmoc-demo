#ifndef KOCMOC_INPUT_GAMEPAD_H_
#define KOCMOC_INPUT_GAMEPAD_H_

#include <common.hpp>

namespace kocmoc
{
	namespace camera
	{
		class FilmCamera;
	}

	namespace input
	{

		/**
		 * Gamepad hanlder class for the 360 gamepad. This class currently only
		 * handles the first joystick that is found, all other are ignored.
		 */
		class Gamepad
		{
		public:
			/**
			 * Construct a new gamepad.
			 */
			Gamepad(camera::FilmCamera* camera);
			~Gamepad(void);

			/**
			 * Detect joystics and setup the controller. Only the first found device
			 * is used. If no devices are found the gamepad is disabled and poll()
			 * return imediatly. Call init() again to rescan/configure the gamepad.
			 * 
			 * @return true iff a gamepad was found.
			 */
			bool init(void);

			/**
			 * poll the device status and react accordingly
			 */
			void poll(void);

		private:
			int numJoystick;
			int numButtons;
			int numAxes;
			GLfloat deadZone;

			bool enableGamepad;

			camera::FilmCamera* camera;
		};
	}
}

#endif