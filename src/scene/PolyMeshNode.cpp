#include "PolyMeshNode.hpp"

#include <Kocmoc.hpp>
#include <util/util.hpp>

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::Shader;

using glm::mat4;


PolyMeshNode::PolyMeshNode(std::string _name)
	: SceneNode(_name)
	, transformation(glm::mat4(1.0f))
{}

PolyMeshNode::~PolyMeshNode(void)
{
	for (std::list<PolyMesh* >::iterator it = polyMeshList.begin(); it != polyMeshList.end(); it++)
	{
		delete [] *it;
	}
}

void PolyMeshNode::add(PolyMesh* mesh)
{
	polyMeshList.push_back(mesh);
}

void PolyMeshNode::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	//for (PolyMeshPointerList::const_iterator ci = polyMeshList.begin(); ci != polyMeshList.end(); ci++)
	//	(*ci)->draw(childTransform, camera, shader);
	
	SceneNode::draw(parentTransform, camera, shader);
}

void PolyMeshNode::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}
