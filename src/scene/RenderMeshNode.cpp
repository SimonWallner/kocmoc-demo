#include "RenderMeshNode.hpp"

#include <Kocmoc.hpp>
#include <util/util.hpp>
#include <renderer/RenderMesh.hpp>

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::Shader;
using kocmoc::renderer::RenderMesh;

using glm::mat4;


RenderMeshNode::RenderMeshNode(std::string _name)
	: SceneNode(_name)
	, transformation(glm::mat4(1.0f))
{}

RenderMeshNode::~RenderMeshNode(void)
{
	for (RenderMeshPointerList::iterator it = renderMeshList.begin();
		it != renderMeshList.end();
		it++)
	{
		delete [] *it;
	}
}

void RenderMeshNode::add(RenderMesh* mesh)
{
	renderMeshList.push_back(mesh);
}

void RenderMeshNode::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	for (RenderMeshPointerList::const_iterator ci = renderMeshList.begin();
		ci != renderMeshList.end();
		ci++)
	{
		(*ci)->draw(childTransform, camera);
	}
	
	SceneNode::draw(parentTransform, camera, shader);
}

void RenderMeshNode::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}
