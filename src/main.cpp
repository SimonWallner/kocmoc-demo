#include "kocmoc.hpp"
#include "PropertiesFileParser.hpp"
#include "Context.hpp"
#include "Exception.hpp"


int main (void)
{
	if (!PropertiesFileParser::GetInstance().parse())
	{
		cout << "CRITICAL: Failed to parse config file, using default values!" << endl;
		cout << "note: values may have been paritally parsed, so not all values are drfaults." << endl;
	}

	try
	{
		Context::getInstance().setupGLFW();
		Context::getInstance().testOpenGL();
		Context::getInstance().setGLStates();

		cout << "glfw/glew/gl setup complete" << endl << endl;

		Kocmoc::getInstance().init();
		Kocmoc::getInstance().start();
		Kocmoc::Destroy();

	} catch (Exception e)
	{
		cout << "A fatal exception was encountered during the execution of the application!" << endl;
		cout << e.getMessage() << endl;
		glfwTerminate();
		return 1;
	}

    glfwTerminate();
    return 0;
}
