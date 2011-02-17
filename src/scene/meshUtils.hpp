#ifndef KOCMOC_SCENE_MESH_UTILS_HPP
#define KOCMOC_SCENE_MESH_UTILS_HPP

#include <glm.hpp>

namespace kocmoc
{
	namespace scene
	{
		class PolyMesh;

		struct SplitResult
		{
			PolyMesh* inside;
			PolyMesh* outside;
		};

		/**
		 * Split the mesh at the given splitPlane
		 * @param d the normal distance origin-plane
		 * @param n the plane normal, it is assumed to point
		 * to the 'outside'
		 */
		SplitResult splitMesh(const PolyMesh* mesh, const double d, const glm::vec3 n);
	}
}

#endif