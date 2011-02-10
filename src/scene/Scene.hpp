#ifndef KOCMOC_SCENE_SCENE_HPP_
#define KOCMOC_SCENE_SCENE_HPP_

#include "PolyMesh.hpp"

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

		class Scene;

		typedef std::list<PolyMesh* > PolyMeshPointerList;
		typedef std::list<Scene* > NodePointerList;

		/**
		 * A loose structure of a scene. No scenegraph, justs lists.
		 */
		class Scene
		{
		public:
			Scene(std::string _name = "scene node");
			~Scene(void);

			void add(PolyMesh* mesh);
			void add(Scene* node);
		
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