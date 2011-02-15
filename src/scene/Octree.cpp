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
	, totalVertexCount(0)
{
	boundingBox = generateUnitCube();
	originGizmo = generateOriginGizmo();
}


void Octree::insert(kocmoc::scene::PolyMesh* mesh)
{
	totalVertexCount += mesh->getVertexCount();

	if (isLeaf)
	{
		content.push_back(mesh);

		if (totalVertexCount > KOCMOC_SCENE_OCTREE_SPLIT_THRESHOLD)
		{
			isLeaf = false;

			children = new Octree*[8];
			children[0] = new Octree(origin + vec3(size/2, size/2, size/2), size/2);
			children[1] = new Octree(origin + vec3(-size/2, size/2, size/2), size/2);
			children[2] = new Octree(origin + vec3(-size/2, -size/2, size/2), size/2);
			children[3] = new Octree(origin + vec3(size/2, -size/2, size/2), size/2);
			children[4] = new Octree(origin + vec3(size/2, size/2, -size/2), size/2);
			children[5] = new Octree(origin + vec3(-size/2, size/2, -size/2), size/2);
			children[6] = new Octree(origin + vec3(-size/2, -size/2, -size/2), size/2);
			children[7] = new Octree(origin + vec3(size/2, -size/2, -size/2), size/2);

			for (uint i = 0; i < content.size(); i++)
				insert(content[i]);

			content.clear();
		}
	}
	else
	{
		vec3 distance = vec3(0) - origin;

		// X
		PolyMesh::SplitResult resultX = mesh->split(distance.x, vec3(1, 0, 0));
		PolyMesh* inside = resultX.inside;
		PolyMesh* outside = resultX.outside;

		// Y
		PolyMesh::SplitResult insideY = inside->split(distance.y, vec3(0, 1, 0));
		PolyMesh* insideInside = insideY.inside;
		PolyMesh* insideOutside = insideY.outside;

		PolyMesh::SplitResult outsideY = outside->split(distance.y, vec3(0, 1, 0));
		PolyMesh* outsideInside = outsideY.inside;
		PolyMesh* outsideOutside = outsideY.outside;

		// Z
		PolyMesh::SplitResult insideInsideZ = insideInside->split(distance.z, vec3(0, 0, 1));
		PolyMesh* iii = insideInsideZ.inside;
		PolyMesh* iio = insideInsideZ.outside;

		PolyMesh::SplitResult insideOutsideZ = insideOutside->split(distance.z, vec3(0, 0, 1));
		PolyMesh* ioi = insideOutsideZ.inside;
		PolyMesh* ioo = insideOutsideZ.outside;

		PolyMesh::SplitResult outsideInsideZ = outsideInside->split(distance.z, vec3(0, 0, 1));
		PolyMesh* oii = outsideInsideZ.inside;
		PolyMesh* oio = outsideInsideZ.outside;

		PolyMesh::SplitResult outsideOutsideZ = outsideOutside->split(distance.z, vec3(0, 0, 1));
		PolyMesh* ooi = outsideOutsideZ.inside;
		PolyMesh* ooo = outsideOutsideZ.outside;

		children[0]->insert(ooo);
		children[0]->insert(ioo);
		children[0]->insert(iio);
		children[0]->insert(oio);
		children[0]->insert(ooi);
		children[0]->insert(ioi);
		children[0]->insert(iii);
		children[0]->insert(oii);
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