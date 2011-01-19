#include "Clock.hpp"

#include "common.hpp"

using namespace kocmoc;

Clock::Clock(void) {}

Clock::~Clock(void) {}


void Clock::start(double _scheduleInterval)
{
	startTime = glfwGetTime();
	lastTickTime = 0.0;
	tick();

	scheduleInterval = _scheduleInterval;
	nextScheduledEvent = scheduleInterval;
}


double Clock::lastFrameDuration()
{
	return frameDuration;
}

void Clock::awaitSchedule()
{
	while (getTime() < nextScheduledEvent)
	{
		glfwSleep(nextScheduledEvent - getTime());
	}
	nextScheduledEvent += scheduleInterval;
}

void Clock::tick()
{
	double currentTime = getTime();
	frameDuration = currentTime - lastTickTime;
	lastTickTime = currentTime;
}

double Clock::getTime()
{
	return glfwGetTime() - startTime;
}