/**
 * Basic timer util
 */

#include "common.hpp"

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