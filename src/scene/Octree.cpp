#include "Octree.hpp"

#include <util/util.hpp>
#include <camera/Camera.hpp>

using namespace kocmoc::scene;

using kocmoc::util::generator::generateOriginGizmo;
using kocmoc::util::generator::generateUnitCube;
using kocmoc::camera::Camera;

using glm::vec3;
using glm::mat4;

using std::vector;

Octree::Octree(vec3 _origin, double _size)
	: origin(_origin)
	, size(_size)
	, children(NULL)
	, isLeaf(true)
{
	boundingBox = generateUnitCube();
	originGizmo = generateOriginGizmo();
}

vector<PolyMesh *> Octree::getContent()
{
	return content;
}

void Octree::insert(kocmoc::scene::PolyMesh* mesh)
{
	content.push_back(mesh);
	
	if (isLeaf)
	{
		children = new Octree*[8];
		children[0] = new Octree(origin + vec3(size/2, size/2, size/2), size/2);
		children[1] = new Octree(origin + vec3(-size/2, size/2, size/2), size/2);
		children[2] = new Octree(origin + vec3(size/2, -size/2, size/2), size/2);
		children[3] = new Octree(origin + vec3(-size/2, -size/2, size/2), size/2);
		children[4] = new Octree(origin + vec3(size/2, size/2, -size/2), size/2);
		children[5] = new Octree(origin + vec3(-size/2, size/2, -size/2), size/2);
		children[6] = new Octree(origin + vec3(size/2, -size/2, -size/2), size/2);
		children[7] = new Octree(origin + vec3(-size/2, -size/2, -size/2), size/2);
		
		isLeaf = false;
	}
}

void Octree::renderDebug(mat4 parentTransform, Camera* camera)
{
	mat4 translate = glm::translate(origin);
	mat4 scale = glm::scale(vec3(size * 2));

	boundingBox->draw(translate * scale, camera);
	originGizmo->draw(translate, camera);

	if (!isLeaf)
	{
		for (uint i = 0; i < 8; i++)
			children[i]->renderDebug(parentTransform, camera);
	}
}