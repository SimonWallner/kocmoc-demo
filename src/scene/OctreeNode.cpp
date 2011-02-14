#include "OctreeNode.hpp"

#include "Octree.hpp"

using namespace kocmoc::scene;

using kocmoc::camera::Camera;
using kocmoc::renderer::Shader;

using glm::mat4;

using std::string;


OctreeNode::OctreeNode(string _name)
	: SceneNode(_name)
{}