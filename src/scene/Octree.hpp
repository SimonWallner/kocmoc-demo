#ifndef KOCMOC_SCENE_OCTREE_HPP_
#define KOCMOC_SCENE_OCTREE_HPP_

/**
 * A Simple octree impelemntation to handle PolyMeshes
 */

#include "PolyMesh.hpp"
#include "LineGizmo.hpp"

#include <types.hpp>

#include <glm.hpp>

#include <vector>

namespace kocmoc
{
	namespace camera
	{
		class Camera;
	}

	namespace scene
	{

		class Octree
		{
		public:

			/**
			 * Construct a new empty Octree. 
			 * @param origin The center of the cube
			 * @param size The extent in each direction from the origin. The volume of
			 * the cube is therefore (2 * size) ^ 3.
			 */
			Octree(glm::vec3 origin, double size);

			/**
			 * Insert a polyMesh into the octree. Upon insertion it might be split and added
			 * to lower levels of the tree.
			 */

			void insert(PolyMesh* mesh);
			
			/**
			 * Get the content, i.e. the PolyMesh in this cell
			 */
			std::vector<PolyMesh* > getContent(void);

			/**
			 * Traverse the octree and return the content as a collection
			 * @param maxRecursionDepth define the maximum number of recursion.
			 */
			std::vector<PolyMesh* > traverse(uint maxRecursionDepth);

			/**
			 * render the contents of this tree, i.e. issue render calls on its
			 * contents.
			 */
			void render(void);

			/**
			 * render debug stuff, like bounding boxes, etc...
			 */
			void renderDebug(glm::mat4 parentTransform, camera::Camera* camera);

		private:
			/** The content of this cell */
			std::vector<PolyMesh* > content;

			/** The child cells */
			Octree* children;

			/** The origin/center of the octree */
			glm::vec3 origin;

			/** The size of the octree. This size is taken as the distance from the 
			 * origin to the perimeter in each dimension. Therefore the octree is
			 * always a regular cube with a volume of (2 * size) ^ 3.
			 */
			double size;

			LineGizmo* boundingBox;
			LineGizmo* originGizmo;
		};
	}
}

#endif