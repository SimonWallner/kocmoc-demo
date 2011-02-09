#include "KocmocScene.hpp"
#include "Kocmoc.hpp"
#include "utility.hpp"

using namespace kocmoc;

KocmocScene::KocmocScene(std::string _name)
	: name(_name)
	, transformation(glm::mat4(1.0f))
{}

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

void KocmocScene::add(KocmocScene* node)
{
	nodeList.push_back(node);
}

void KocmocScene::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	for (PolyMeshPointerList::const_iterator ci = polyMeshList.begin(); ci != polyMeshList.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
	
	for (NodePointerList::const_iterator ci = nodeList.begin(); ci != nodeList.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
}

void KocmocScene::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}

void KocmocScene::setTransformation(glm::core::type::mat4 t)
{
	transformation = t;
}