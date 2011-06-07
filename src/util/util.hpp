#ifndef KOCMOC_UTIL_UTIL_HPP_
#define KOCMOC_UTIL_UTIL_HPP_

#include <types.hpp>
#include <Exception.hpp>

#include <glm.hpp>

#include <vector>


namespace kocmoc
{
	namespace scene
	{
		class LineGizmo;
	}

	namespace renderer
	{
		class RenderMesh;
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
			kocmoc::renderer::RenderMesh* generateStars();

			/**
			 * Generate a colored coordinate cross gizmo
			 */
			kocmoc::scene::LineGizmo* generateOriginGizmo();

			/**
			 * Generates a unit cube to arround the origin 
			 */
			kocmoc::scene::LineGizmo* generateUnitCube(glm::vec3 color = glm::vec3(1));

			kocmoc::renderer::RenderMesh* generateTriangle();
		}

		namespace geometry
		{
			/**
			 * test if a point is inside, i.e. not on the side of the 
			 * normal vector that is pointing towards the outside
			 * @return true iff the point is inside or directly on the plane
			 */
			bool inside(const double d, const glm::dvec3& n, const glm::dvec3& point);

			/**
			 * calculate the intersection parameter for a later
			 * linear mix. It is assumed that there is a valid 
			 * intersection point!
			 * @return 0 iff the intersection is in p1, 1 iff the intersection is in
			 *		p2, (0, 1) in between.
			 */
			double intersect(const double d, const glm::dvec3& n, const glm::dvec3& p1, const glm::dvec3& p2);

			/**
			 * linear combination 
			 * @return v1 iff r == 0, v2 iff r == 1, and a mix if r is in (0, 1).
			 */
			template <typename T >
			T lerp(const double r, const T& v1, const T& v2)
			{
				return (v1 * (1-r)) + (v2 * r);
			}
		}

		namespace gl
		{
			/**
			 * issues a glFinish call and times the time it takes to return from
			 * it. This should give an approximate figure about the pipeline
			 * condition.
			 */
			void timedFinish(void);
		}

		namespace math
		{
			template <typename T >
			int sign(T x)
			{
				return (x > 0) - (x < 0);
				// no branching sign, found on stack overflow
			}

			template <typename T >
			T max(T v1, T v2)
			{
				return (v1 > v2 ? v1 : v2);
			}

			template <typename T >
			T min(T v1, T v2)
			{
				return (v1 < v2 ? v1 : v2);
			}
		}

		namespace parser
		{
			/**
			 * parse the shader and resolve #pragma includes
			 * @param shaderName the file name of the shader
			 * @param pathPrefix the local path to the include directory
			 * @param includeCounter the number of the included file, used
			 *		for the #line annotation. lineNumber:includeNumber.
			 *		Base file should be 0.
			 * @return a string with resolved includes and #line annotations
			 */
			std::string parseShader(std::string shaderName, std::string pathPrefix, uint includeCounter = 0) throw(kocmoc::Exception);
		}
	}
}


#endif