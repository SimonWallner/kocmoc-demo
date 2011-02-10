#ifndef KOCMOC_TIME_ANIMATION_CLOCK_HPP_
#define KOCMOC_TIME_ANIMATION_CLOCK_HPP_

namespace kocmoc
{
	namespace time
	{
		class Clock;

		/**
		 * A clock for animation that supports start stop, speed, shuttling,
		 * forward rewind, etc...
		 * All times are based seconds.
		 */
		class AnimationClock
		{
		public:

			/**
			 * Construct a new instance of the animation clock
			 * @param referenceClock The clock that is used as the reference and 
			 * where all time measurements are based on. This clock should have
			 * a constant speed in most usage cases.
			 */ 
			AnimationClock(Clock *referenceClock);
			~AnimationClock(void);

			/**
			 * set the current speed of the clock.
			 * @param speed all values are allowed, set it to 0 to pause the clock
			 */
			void setSpeed(double speed);

			/** set the standard playback speed */
			void setPlaybackSpeed(double speed);

			/**
			 * start the clock at standard playback speed. I.e set its speed to it
			 */
			void play(void);

			/**
			 * get the current time of the clock.
			 * These values are neither monotonous nor linear, nor anysting else.
			 */
			double getTime(void);

			/** Set the clock to the given time
			 * @param time an arbitrary time, even negative
			 */
			void setTime(double time);


		private:
			/** current speed */
			double speed;

			/** the standard speed, i.e the speed of normal "play" */
			double playbackSpeed;

			/** the current time of this clock */
			double currentTime;

			Clock *referenceClock;

			double lastReferenceTime;
		};
	}
}

#endif