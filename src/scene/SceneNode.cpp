#include "SceneNode.hpp"

#include <Kocmoc.hpp>
#include <util/util.hpp>

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::Shader;

using glm::mat4;


SceneNode::SceneNode(std::string _name)
	: name(_name)
	, transformation(glm::mat4(1.0f))
{}

SceneNode::~SceneNode(void)
{
	for (std::list<PolyMesh* >::iterator it = polyMeshList.begin(); it != polyMeshList.end(); it++)
	{
		delete [] *it;
	}
}

void SceneNode::add(PolyMesh* mesh)
{
	polyMeshList.push_back(mesh);
}

void SceneNode::add(SceneNode* node)
{
	nodeList.push_back(node);
}

void SceneNode::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	for (PolyMeshPointerList::const_iterator ci = polyMeshList.begin(); ci != polyMeshList.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
	
	for (NodePointerList::const_iterator ci = nodeList.begin(); ci != nodeList.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
}

void SceneNode::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}

void SceneNode::setTransformation(glm::core::type::mat4 t)
{
	transformation = t;
}