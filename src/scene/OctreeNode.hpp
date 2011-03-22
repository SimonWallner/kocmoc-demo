#ifndef KOCMOC_SCENE_OCTREE_NODE_HPP_
#define KOCMOC_SCENE_OCTREE_NODE_HPP_

#include "SceneNode.hpp"
#include <Symbol.hpp>

namespace kocmoc
{
	namespace renderer
	{
		class RenderMesh;
	}

	namespace scene
	{
		class Octree;

		class OctreeNode : public SceneNode
		{
		public:
			OctreeNode(Octree* octree, std::string name = "octree node");

			void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL);
			void draw(camera::Camera* camera, renderer::Shader* shader = NULL);
			
			void add(SceneNode* node) {SceneNode::add(node);};

		private:
			Octree* octree;
		};
	}
}

#endif