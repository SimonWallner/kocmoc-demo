#include "Octree.hpp"

using namespace kocmoc::scene;

using std::vector;
using glm::vec3;

Octree::Octree(glm::vec3 _origin, double _size)
	: origin(_origin)
	, size(_size)
	, children(NULL)
{}

vector<PolyMesh *> Octree::getContent()
{
	return content;
}

void Octree::insert(kocmoc::scene::PolyMesh* mesh)
{
	content.push_back(mesh);
}