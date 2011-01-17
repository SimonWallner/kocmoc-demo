#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "common.hpp"

namespace kocmoc
{

	/**
	 * This singleton encapsulates all the magic that is needed to create our main
	 * window and context. Since it is a singleton you can not construct one but
	 * use the static getInstance() method to get a pointer at the contex.
	 *
	 * WARNING: not thread safe!
	 */
	class Context
	{
	public:
		/**
		 * Get the a pointer to the instance of the Context.
		 * @return the pointer to the instance.
		 */
		static Context& getInstance(void);

		~Context(void);

		/**
		 * Set up glfw window
		 */
		void setupGLFW(void);

		/**
		 * Test for OpenGL 3 and a FWCC
		 */
		void testOpenGL(void);

		/**
		 * Set necessary gl states (enable textures, etc)
		 */
		void setGLStates(void);

		/**
		 * swap the back buffer for the fron buffer and vice versa
		 */
		void swapBuffers(void);

		/**
		 * Clear some buffers
		 */
		void clearBuffers(void);

		/**
		 * Togglet the wireframe mode
		 */
		void setWireframe(bool enable);
		bool getWireframe(void) {return wireframeMode;};

		void getAvailableResolutions(void);


		int width, height;


	private:
		Context(void);
		Context(const Context &cc); // protect the copy constructor as well
		bool wireframeMode;

	};
}

#endif
