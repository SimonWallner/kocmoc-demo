#ifndef _KOCMOC_SCENE_HPP_
#define _KOCMOC_SCENE_HPP_

#include "common.hpp"
#include "PolyMesh.hpp"
#include <list>
#include "Shader.hpp"
#include "Camera.hpp"

namespace kocmoc
{
	class KocmocScene;

	typedef std::list<PolyMesh* > PolyMeshPointerList;
	typedef std::list<KocmocScene* > NodePointerList;

	/**
	 * A loose structure of a scene. No scenegraph, justs lists.
	 */
	class KocmocScene
	{
	public:
		KocmocScene(std::string _name = "scene node");
		~KocmocScene(void);

		void add(PolyMesh* mesh);
		void add(KocmocScene* node);
	
		void draw(glm::mat4 transform, Camera *camera);
		void draw(Camera *camera);

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
#endif
