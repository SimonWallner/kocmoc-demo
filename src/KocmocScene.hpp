#ifndef _KOCMOC_SCENE_HPP_
#define _KOCMOC_SCENE_HPP_

#include "common.hpp"
#include "PolyMesh.hpp"
#include <list>
#include "Shader.hpp"

/**
 * A loose structure of a scene. No scenegraph, justs lists.
 */
class KocmocScene
{
public:
	KocmocScene() : name("") {};
	KocmocScene(string _name) : name(_name) {};
	~KocmocScene(void);

	void addPolyMesh(PolyMesh* mesh);
	std::list<PolyMesh* > getPolyMeshList(void);

	void transferData(Shader shader);
	void draw(void);

private:
	std::list<PolyMesh* > polyMeshList;
	string name;
	GLuint **vao_ids; 
};

#endif