#ifndef _UTILITY_HPP_
#define _UTILITY_HPP_

#include "common.hpp"
#include "PolyMesh.hpp"

#include <string>
#include <vector>

namespace kocmoc
{
	namespace util
	{

		/**
		 * Check if the given file exists on the file system.
		 */
		bool file_exists(const std::string &filename);

		/**
		 * Read the given file into a String
		 */
		string read_file(const std::string &filename);

		/**
		 * Query OpenGL errors and print error messages to STDERR.
		 */
		//void get_errors(void);

		/**
		 * Safe delete an array
		 * only delete if not null and set null afterwards
		 */
		void safeDeleteArray(void *p);

		void tokenize(const std::string& str, std::vector<std::string >& tokens, const std::string delimiter);

		namespace generator
		{
			/**
			 * generate a starfield and take the needed params from the config
			 * file
			 */
			PolyMesh* generateStars();

			/**
			 * Generate a colored coordinate cross gizmo
			 */
			PolyMesh* generateGizmo();
		}
	}
}


#endif
