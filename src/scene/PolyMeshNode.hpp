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
			typedef std::list<SceneNode* > NodePointerList;

		public:
			PolyMeshNode(std::string _name = "scene node");
			~PolyMeshNode(void);

			void add(PolyMesh* mesh);
			void add(SceneNode* node);
		
			void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL);
			void draw(camera::Camera* camera, renderer::Shader* shader = NULL);

			void setTransformation(glm::mat4 transform);
			glm::mat4 getTransformation(void) {return transformation;};

		private:
			PolyMeshPointerList polyMeshList;
			NodePointerList nodeList;
			std::string name;

			/** The transformation matrix of this node */
			glm::mat4 transformation;
		};
	}
}
#endif