#include "AnimationClock.hpp"

using namespace kocmoc;


AnimationClock::AnimationClock(Clock *_referenceClock)
	: referenceClock(_referenceClock)
	, playbackSpeed(1.0)
	, speed(0.0)
{}

AnimationClock::~AnimationClock(void) {}

double AnimationClock::getTime()
{
	double currentReferenceTime = referenceClock->getTime();
	currentTime += speed * (currentReferenceTime - lastReferenceTime);
	lastReferenceTime = currentReferenceTime;

	return currentTime;
}