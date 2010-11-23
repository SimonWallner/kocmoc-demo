#include "KocmocScene.hpp"

KocmocScene::~KocmocScene(void)
{
	for (std::list<PolyMesh* >::iterator it = polyMeshList.begin(); it != polyMeshList.end(); it++)
	{
		delete [] *it;
	}
}

void KocmocScene::addPolyMesh(PolyMesh* mesh)
{
	polyMeshList.push_back(mesh);
}

std::list<PolyMesh*> KocmocScene::getPolyMeshList()
{
	return polyMeshList;
}

void KocmocScene::transferData(Shader shader)
{
	shader.bind();
	vao_ids = new GLuint [polyMeshList.size()];
	glGenVertexArrays(polyMeshList.size(), vao_ids);

	int i = 0;
	for (std::list<PolyMesh* >::iterator it = polyMeshList.begin(); it != polyMeshList.end(); it++)
	{
		(*it)->transferData(shader, &vao_ids[i]);
		i++;
	}
	shader.unbind();
}

void KocmocScene::draw()
{
	for (unsigned int i = 0; i < polyMeshList.size(); i++)
	{
		glBindVertexArray(vao_ids[i]);
	get_errors();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	get_errors();
		glBindVertexArray(0);
		get_errors();
	}
}