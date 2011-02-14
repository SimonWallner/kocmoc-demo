#include "SceneNode.hpp"

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::Shader;

void SceneNode::add(SceneNode* node)
{
	children.push_back(node);
}

void SceneNode::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	for (NodePointerList::const_iterator ci = children.begin(); ci != children.end(); ci++)
		(*ci)->draw(childTransform, camera, shader);
}

void SceneNode::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}