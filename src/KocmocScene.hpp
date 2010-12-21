#ifndef _KOCMOC_SCENE_HPP_
#define _KOCMOC_SCENE_HPP_

#include "common.hpp"
#include "PolyMesh.hpp"
#include <list>
#include "Shader.hpp"

namespace kocmoc
{
	typedef std::list<PolyMesh* > PolyMeshPointerList;

	/**
	 * A loose structure of a scene. No scenegraph, justs lists.
	 */
	class KocmocScene
	{
	public:
		KocmocScene();
		KocmocScene(string _name) : name(_name) {};
		~KocmocScene(void);

		void add(PolyMesh* mesh);
	
		void draw();

	private:
		PolyMeshPointerList polyMeshList;
		std::string name;
	};
}
#endif
