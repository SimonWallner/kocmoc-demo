#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include "FilmCamera.hpp"

namespace kocmoc
{
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
			Gamepad(FilmCamera* camera);
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

			FilmCamera* camera;
		};
	}
}

#endif
