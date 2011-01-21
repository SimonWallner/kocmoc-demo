#include "AnimationClock.hpp"

using namespace kocmoc;


AnimationClock::AnimationClock(Clock *_referenceClock)
	: referenceClock(_referenceClock)
	, playbackSpeed(1.0)
	, speed(0.0)
	, currentTime(0.0)
{
	lastReferenceTime = referenceClock->getTime();
}

AnimationClock::~AnimationClock(void) {}

double AnimationClock::getTime()
{
	double currentReferenceTime = referenceClock->getTime();
	currentTime += speed * (currentReferenceTime - lastReferenceTime);
	lastReferenceTime = currentReferenceTime;

	return currentTime;
}

void AnimationClock::setPlaybackSpeed(double speed)
{
	playbackSpeed = speed;
}

void AnimationClock::setSpeed(double _speed)
{
	getTime(); // needed to update lastReferenceTime, clean up with the old speed and set new
	speed = _speed;
}

void AnimationClock::play()
{
	setSpeed(playbackSpeed);
}