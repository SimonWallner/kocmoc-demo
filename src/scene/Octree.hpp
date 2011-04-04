#ifndef KOCMOC_SCENE_OCTREE_HPP_
#define KOCMOC_SCENE_OCTREE_HPP_

/**
 * A Simple octree impelemntation to handle PolyMeshes
 */

#include "LineGizmo.hpp"

#include <types.hpp>

#include <glm.hpp>

#include <vector>

#define KOCMOC_SCENE_OCTREE_SPLIT_THRESHOLD 0

namespace kocmoc
{
	namespace camera
	{
		class Camera;
	}

	namespace renderer
	{
		class RenderMesh;
	}

	namespace scene
	{
		/**
		 * an Octree to handle poly meshes
		 * 
		 * Octantans are named/ordered and handled as follows
		 * Ocant | X | Y | Z |
		 * -------------------
		 *  I    | + | + | + |
		 *  II   | - | + | + |
		 *  III  | - | - | + |
		 *  IV   | + | - | + |
		 *  V    | + | + | - |
		 *  VI   | - | + | - |
		 *  VII  | - | - | - |
		 *  VIII | + | - | - |
		 */
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
			 * @param maxRecursionDepth limit the recursion depth, default = -1 for
			 *		infinite recursions. If 0 is reached the method returns.
			 */

			void insert(renderer::RenderMesh* mesh, int maxRecursionDepth = -1);
			

			/**
			 * render the contents of this tree, i.e. issue render calls on its
			 * contents.
			 */
			void draw(glm::mat4 parentTransform, camera::Camera* camera, bool cull = true);

			/**
			 * render debug stuff, like bounding boxes, etc...
			 */
			void drawDebug(glm::mat4 parentTransform, camera::Camera* camera);

		private:

			typedef std::vector<renderer::RenderMesh* > ContentList;

			/** The content of this cell */
			ContentList content;

			/** The total vertex count of of the subtree (splitting overhead 
			 * is not counted. */
			uint totalVertexCount;

			/** The child cells */
			Octree** children;

			/** The origin/center of the octree */
			glm::vec3 origin;

			/** The size of the octree. This size is taken as the distance from the 
			 * origin to the perimeter in each dimension. Therefore the octree is
			 * always a regular cube with a volume of (2 * size) ^ 3.
			 */
			double size;

			LineGizmo* boundingBox;
			LineGizmo* originGizmo;

			bool isLeaf;

			/**
			 * Test if the AABB is visible in the given projection
			 * brute force implementation. Project corners and then check each 
			 * corner against every plane.
			 */
			bool isVisible(const glm::mat4 projection);

		};
	}
}

#endif