#include "KocmocScene.hpp"

KocmocScene::~KocmocScene(void)
{
}

void KocmocScene::addPolyMesh(PolyMesh mesh)
{
	polyMeshList.push_back(mesh);
}

std::list<PolyMesh> KocmocScene::getPolyMeshList()
{
	return polyMeshList;
}
