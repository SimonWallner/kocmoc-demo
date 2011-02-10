#ifndef KOCMOC_UTIL_UTIL_HPP_
#define KOCMOC_UTIL_UTIL_HPP_

//#include <scene/PolyMesh.hpp>

#include <vector>




namespace kocmoc
{
	namespace scene
	{
		class PolyMesh;
	}

	namespace util
	{

		/**
		 * Check if the given file exists on the file system.
		 */
		bool file_exists(const std::string &filename);

		/**
		 * Read the given file into a String
		 */
		std::string read_file(const std::string &filename);

		/**
		 * Safe delete an array
		 * only delete if not null and set null afterwards
		 */
		void safeDeleteArray(void *p);

		void tokenize(const std::string& str, std::vector<std::string >& tokens, const std::string& delimiter);

		namespace generator
		{
			/**
			 * generate a starfield and take the needed params from the config
			 * file
			 */
			kocmoc::scene::PolyMesh* generateStars();

			/**
			 * Generate a colored coordinate cross gizmo
			 */
			kocmoc::scene::PolyMesh* generateGizmo();
		}
	}
}


#endif