#ifndef KOCMOC_SCENE_SCENE_NODE_HPP_
#define KOCMOC_SCENE_SCENE_NODE_HPP_

#include <glm.hpp>

#include <string>
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
		 * An abstract scene node base class. It has a transformation matrix 
		 * and an arbitrary number of childnodes.
		 */
		class SceneNode
		{
			typedef std::list<SceneNode* > NodePointerList;

		public:
			SceneNode(std::string _name) : name(_name) {};
			virtual ~SceneNode(void){};

			virtual void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL);
			void draw(camera::Camera* camera, renderer::Shader* shader = NULL);

			void add(SceneNode* node);

			void setTransformation(glm::mat4 t) {transformation = t;};
			glm::mat4 getTransformation() {return transformation;};
			
			void setName(std::string n) {name = n;};
			std::string getName(void) {return name;};

		protected:
			glm::mat4 transformation;
			std::string name;
			NodePointerList children;
		};
	}
}

#endif