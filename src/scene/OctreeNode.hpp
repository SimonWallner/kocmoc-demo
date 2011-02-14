#ifndef KOCMOC_SCENE_OCTREE_NODE_HPP_
#define KOCMOC_SCENE_OCTREE_NODE_HPP_

#include "SceneNode.hpp"

namespace kocmoc
{
	namespace scene
	{
		class Octree;

		class OctreeNode : public SceneNode
		{
		public:
			OctreeNode(std::string name = "octree node");

			//void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL);
			//void draw(camera::Camera* camera, renderer::Shader* shader = NULL);

		private:
			Octree* octree;
		};
	}
}

#endif