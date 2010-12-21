#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include "common.hpp"

#include <string>
using std::string;

namespace kocmoc
{
	namespace util
	{

		/**
		 * Check if the given file exists on the file system.
		 */
		bool file_exists(const string &filename);

		/**
		 * Read the given file into a String
		 */
		string read_file(const string &filename);

		/**
		 * Query OpenGL errors and print error messages to STDERR.
		 */
		//void get_errors(void);

		/**
		 * Safe delete an array
		 * only delete if not null and set null afterwards
		 */
		void safeDeleteArray(void *p);
	}
}


#endif
