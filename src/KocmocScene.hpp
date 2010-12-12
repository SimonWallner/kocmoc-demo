#ifndef _KOCMOC_SCENE_HPP_
#define _KOCMOC_SCENE_HPP_

#include "common.hpp"
#include "PolyMesh.hpp"
#include <list>
#include "shader.hpp"

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
	std::list<PolyMesh* > polyMeshList;
	string name;
	GLuint *vao_ids;
	unsigned int * vertexCounts;

	GLuint cross_vao;
	GLuint *cross_vbos;
	Shader *crossShader;
};

#endif
