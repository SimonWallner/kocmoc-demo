#ifndef KOCMOC_TIME_CLOCK_HPP_
#define KOCMOC_TIME_CLOCK_HPP_

namespace kocmoc
{
	namespace time
	{

		/** 
		 * A simple clock class to get various timing infos, total
		 * runtime, time since last frame, etc...
		 * All values are in double precission (for what its worth).
		 * All values are in SECONDS undless otherwise noticed.
		 */
		class Clock
		{
		public:
			Clock(void);
			~Clock(void);


			/**
			 * Start the clock with a schedule interval. It is used in the 
			 * awaitSchedule() function.
			 */
			void start(double scheduleInterval = 0.0);

			/**
			 * advance timing to the next frame
			 */
			void tick(void);

			/**
			 * Get the duration of the last frame. I.e. the time between the last
			 * two ticks()
			 */
			double lastFrameDuration(void);

			/**
			 * Wait for the next scheduled timeframe to happen.
			 * A regularily spaced schedule can be set in the start method.
			 * Use this for fixed framerates and things alike
			 */
			void awaitSchedule(void);

			/**
			 * Get the time since start()
			 */
			double getTime(void);

		private:

			/** duration of the last frame, i.e. the interval between the last
			 * two ticks */
			double frameDuration;

			/** The time of the last tick */
			double lastTickTime;

			/** The time of the start of the clock. Used to base the 0 */
			double startTime;

			double scheduleInterval;
			double nextScheduledEvent;
			double lastScheduledEvent;
		};
	}
}

#endif