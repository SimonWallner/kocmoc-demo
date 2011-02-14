#ifndef KOCMOC_SCENE_POLY_MESH_NODE_HPP_
#define KOCMOC_SCENE_POLY_MESH_NODE_HPP_

#include "PolyMesh.hpp"
#include "SceneNode.hpp"

#include <common.hpp>

#include <list>

namespace kocmoc
{
	namespace camera
	{
		class Camera;
	}

	namespace renderer
	{
		class Shader;
	}

	namespace scene
	{

		/**
		 * ...
		 */
		class PolyMeshNode : public SceneNode
		{
			typedef std::list<PolyMesh* > PolyMeshPointerList;
			
		public:
			PolyMeshNode(std::string _name = "scene node");
			~PolyMeshNode(void);

			void add(SceneNode* node) {SceneNode::add(node);};
			void add(PolyMesh* mesh);
		
			void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL);
			void draw(camera::Camera* camera, renderer::Shader* shader = NULL);

		private:
			PolyMeshPointerList polyMeshList;

			/** The transformation matrix of this node */
			glm::mat4 transformation;
		};
	}
}
#endif