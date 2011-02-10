#ifndef KOCMOC_EXCEPTION_H_
#define KOCMOC_EXCEPTION_H_

#include "common.hpp"

namespace kocmoc
{

	/**
	 * Base exception class. Supports a single exception, no nested exceptions
	 * at this level. No stack traces are supported in c++ :-(
	 * Exceptions are immutable.
	 */
	class Exception
	{
	public:
		/**
		 * Construct a new exception with the given message
		 * @param msg The message of the exception. This message shoul me 1.)
		 * human readable, and 2.) informative.
		 */
		Exception(std::string msg);
		~Exception(void);

		/**
		 * Get the exception message.
		 * @return The exception message in human readable and hopefully informative
		 *		form
		 */
		std::string getMessage(void);

	private:
		std::string message;
	};
}

#endif