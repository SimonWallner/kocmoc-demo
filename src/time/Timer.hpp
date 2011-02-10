#ifndef KOCMOC_TIME_TIMER_HPP_
#define KOCMOC_TIME_TIMER_HPP_

#include <common.hpp>

namespace kocmoc
{
	namespace time
	{

		/**
		 * Basic timer util
		 */
		class Timer
		{
		public:
			Timer(void);
			~Timer(void);
		
			/**
			 * Hit the timer
			 */
			void tic(void);

			/**
			 * print data to stdout
			 */
			void print(void);

		private:
			unsigned long counter;
			GLdouble * frameTimes;
			GLdouble lastFrame;
		};
	}
}

#endif
