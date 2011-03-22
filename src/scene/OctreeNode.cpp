#include "OctreeNode.hpp"

#include "Octree.hpp"

#include <util/Property.hpp>
#include <Kocmoc.hpp>
#include <Symbol.hpp>

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::Shader;
using kocmoc::util::Property;
using kocmoc::Kocmoc;
using kocmoc::symbolize;

using glm::mat4;

using std::string;


OctreeNode::OctreeNode(Octree* _octree, string _name)
	: octree(_octree)
	, SceneNode(_name)
{}


void OctreeNode::draw(glm::mat4 parentTransform, Camera *camera, Shader *shader)
{
	glm::mat4 childTransform = parentTransform * transformation;

	octree->draw(childTransform, camera);

	if (Kocmoc::paramMapBool[symbolize("debugDrawOctreeGizmo")])
		octree->drawDebug(childTransform, camera);

	// pass the parent transform because the super method will compute the 
	// child transform on its own.
	SceneNode::draw(parentTransform, camera, shader);
}

void OctreeNode::draw(Camera *camera, Shader *shader)
{
	draw(glm::mat4(1.0f), camera, shader);
}
