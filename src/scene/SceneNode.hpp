#ifndef KOCMOC_SCENE_SCENE_NODE_HPP_
#define KOCMOC_SCENE_SCENE_NODE_HPP_


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
		public:
			virtual ~SceneNode(void){};

			virtual void add(SceneNode* node) = 0;

			virtual void draw(glm::mat4 transform, camera::Camera* camera, renderer::Shader* shader = NULL) = 0;
			virtual void draw(camera::Camera* camera, renderer::Shader* shader = NULL) = 0;

			void setTransformation(glm::mat4 t) {transformation = t;};
			glm::mat4 getTransformation() {return transformation;};

		protected:
			glm::mat4 transformation;
		};
	}
}

#endif