#ifndef KOCMOC_SCENE_RENDER_MESH_NODE_HPP_
#define KOCMOC_SCENE_RENDER_MESH_NODE_HPP_

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
		class RenderMesh;
	}

	namespace scene
	{

		/**
		 * ...
		 */
		class RenderMeshNode : public SceneNode
		{
			typedef std::list<renderer::RenderMesh* > RenderMeshPointerList;
			
		public:
			RenderMeshNode(std::string _name = "render mesh node");
			~RenderMeshNode(void);

			void add(SceneNode* node) {SceneNode::add(node);};
			void add(renderer::RenderMesh* mesh);
		
			void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL);
			void draw(camera::Camera* camera, renderer::Shader* shader = NULL);

		private:
			RenderMeshPointerList renderMeshList;

			/** The transformation matrix of this node */
			glm::mat4 transformation;
		};
	}
}
#endif