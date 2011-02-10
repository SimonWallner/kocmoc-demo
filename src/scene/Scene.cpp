#include "Scene.hpp"

#include <Kocmoc.hpp>
#include <util/util.hpp>

using namespace kocmoc;

Scene::Scene(std::string _name)
	: name(_name)
	, transformation(glm::mat4(1.0f))
{}

Scene::~Scene(void)
{
	for (std::list<PolyMesh* >::iterator it = polyMeshList.begin(); it != polyMeshList.end(); it++)
	{
		delete [] *it;
	}
}

void Scene::add(PolyMesh* mesh)
{
	polyMeshList.push_back(mesh);
}

void Scene::add(Scene* node)
{
	nodeList.push_back(node);
}

void Scene::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	for (PolyMeshPointerList::const_iterator ci = polyMeshList.begin(); ci != polyMeshList.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
	
	for (NodePointerList::const_iterator ci = nodeList.begin(); ci != nodeList.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
}

void Scene::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}

void Scene::setTransformation(glm::core::type::mat4 t)
{
	transformation = t;
}