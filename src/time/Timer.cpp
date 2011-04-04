#include "Timer.hpp"

#define AVERAGE_N_FRAMES 10

using namespace kocmoc::time;

Timer::Timer()
{
	counter = 0;
	frameTimes = new GLdouble[AVERAGE_N_FRAMES];
	for (unsigned int i = 0; i < AVERAGE_N_FRAMES; i++)
		frameTimes[i] = 0.0;

	lastFrame = glfwGetTime();
}

Timer::~Timer(void)
{
	delete [] frameTimes;
}

void Timer::tic()
{
	counter++;
	
	int index = counter % AVERAGE_N_FRAMES;
	GLdouble delta = glfwGetTime() - lastFrame; 
	frameTimes[index] = delta;

	lastFrame = glfwGetTime();
	print();
}

void Timer::print(void)
{
	float sum = 0;
	for (unsigned int i = 0; i < AVERAGE_N_FRAMES; i++)
		sum += frameTimes[i];

	float average = sum / AVERAGE_N_FRAMES;
	char buff[50];
	sprintf(buff, "%4.3f ms | %4.2f fps",  average * 1000, 1 / average);
	glfwSetWindowTitle(buff);
	//std::cout << average * 1000 << "ms | " << 1 / average << "fps" << std::endl;
}
