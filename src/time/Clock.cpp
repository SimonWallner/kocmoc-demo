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
	lastScheduledEvent = 0.0;
	nextScheduledEvent = scheduleInterval;
}


double Clock::lastFrameDuration()
{
	if (scheduleInterval != 0.0)
		return scheduleInterval;
	else
		return frameDuration;
}

void Clock::awaitSchedule()
{
	while ((glfwGetTime() - startTime) < nextScheduledEvent)
	{
		glfwSleep(nextScheduledEvent - (glfwGetTime() - startTime));
	}
	lastScheduledEvent = nextScheduledEvent;
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
	if (scheduleInterval != 0.0)
		return lastScheduledEvent;
	else
		return glfwGetTime() - startTime;
}