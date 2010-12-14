#ifndef _KOCMOC_SCENE_HPP_
#define _KOCMOC_SCENE_HPP_

#include "common.hpp"
#include "PolyMesh.hpp"
#include <list>
#include "shader.hpp"

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

	void addPolyMesh(PolyMesh* mesh);
	std::list<PolyMesh* > getPolyMeshList(void);

	void transferData(Shader *shader);
	void draw(Shader* shader);

private:
	PolyMeshPointerList polyMeshList;
	string name;

	GLuint gizmo_vao;
	GLuint *gizmo_vbos;
	Shader *gizmoShader;
};

#endif
