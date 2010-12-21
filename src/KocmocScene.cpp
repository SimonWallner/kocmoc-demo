#include "KocmocScene.hpp"
#include "Kocmoc.hpp"
#include "utility.hpp"

using namespace kocmoc;

KocmocScene::KocmocScene() : name("Scene")
{
	gizmo = util::generator::generateGizmo();
}

KocmocScene::~KocmocScene(void)
{
	for (std::list<PolyMesh* >::iterator it = polyMeshList.begin(); it != polyMeshList.end(); it++)
	{
		delete [] *it;
	}
}

void KocmocScene::add(PolyMesh* mesh)
{
	polyMeshList.push_back(mesh);
}

void KocmocScene::draw()
{
	for (PolyMeshPointerList::const_iterator ci = polyMeshList.begin(); ci != polyMeshList.end(); ci++)
	{
		(*ci)->draw();
	}

	gizmo->draw();
}