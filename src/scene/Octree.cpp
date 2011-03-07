#include "Octree.hpp"

#include <util/util.hpp>
#include <camera/Camera.hpp>
#include <renderer/RenderMesh.hpp>
#include <scene/meshUtils.hpp>

using namespace kocmoc::scene;

using kocmoc::util::generator::generateOriginGizmo;
using kocmoc::util::generator::generateUnitCube;
using kocmoc::camera::Camera;
using kocmoc::renderer::RenderMesh;
using kocmoc::scene::PolyMesh;
using kocmoc::scene::meshUtils::SplitResult;
using kocmoc::scene::meshUtils::splitMesh;

using glm::vec3;
using glm::dvec3;
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


void Octree::insert(RenderMesh* mesh)
{
	if (mesh == NULL) 
		return;


	if (isLeaf)
	{
		content.push_back(mesh);

		totalVertexCount += mesh->mesh->vertexIndexCount;
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
		SplitResult resultX = splitMesh(mesh->mesh, distance.x, dvec3(1, 0, 0));
		PolyMesh* inside = resultX.inside;
		PolyMesh* outside = resultX.outside;

		// Y
		SplitResult insideY = splitMesh(inside, distance.y, dvec3(0, 1, 0));
		PolyMesh* insideInside = insideY.inside;
		PolyMesh* insideOutside = insideY.outside;

		SplitResult outsideY = splitMesh(outside, distance.y, dvec3(0, 1, 0));
		PolyMesh* outsideInside = outsideY.inside;
		PolyMesh* outsideOutside = outsideY.outside;

		// Z
		SplitResult insideInsideZ = splitMesh(insideInside, distance.z, dvec3(0, 0, 1));
		PolyMesh* iii = insideInsideZ.inside;
		PolyMesh* iio = insideInsideZ.outside;

		SplitResult insideOutsideZ = splitMesh(insideOutside, distance.z, dvec3(0, 0, 1));
		PolyMesh* ioi = insideOutsideZ.inside;
		PolyMesh* ioo = insideOutsideZ.outside;

		SplitResult outsideInsideZ = splitMesh(outsideInside, distance.z, dvec3(0, 0, 1));
		PolyMesh* oii = outsideInsideZ.inside;
		PolyMesh* oio = outsideInsideZ.outside;

		SplitResult outsideOutsideZ = splitMesh(outsideOutside, distance.z, dvec3(0, 0, 1));
		PolyMesh* ooi = outsideOutsideZ.inside;
		PolyMesh* ooo = outsideOutsideZ.outside;

		if (ooo != NULL)
			children[0]->insert(new RenderMesh(ooo, mesh->shader));
		if (ioo != NULL)
			children[0]->insert(new RenderMesh(ioo, mesh->shader));
		if (iio != NULL)
			children[0]->insert(new RenderMesh(iio, mesh->shader));
		if (oio != NULL)
			children[0]->insert(new RenderMesh(oio, mesh->shader));
		if (ooi != NULL)
			children[0]->insert(new RenderMesh(ooi, mesh->shader));
		if (ioi != NULL)
			children[0]->insert(new RenderMesh(ioi, mesh->shader));
		if (iii != NULL)
			children[0]->insert(new RenderMesh(iii, mesh->shader));
		if (oii != NULL)
			children[0]->insert(new RenderMesh(oii, mesh->shader));
	}
}

void Octree::drawDebug(mat4 parentTransform, Camera* camera)
{
	mat4 translate = glm::translate(origin);
	mat4 scale = glm::scale(vec3(size * 2));
	mat4 originScale = glm::scale(vec3(size / 4));

	boundingBox->draw(translate * scale, camera);
	originGizmo->draw(translate * originScale, camera);

	if (!isLeaf)
	{
		for (uint i = 0; i < 8; i++)
			children[i]->drawDebug(parentTransform, camera);
	}
}

void Octree::draw(mat4 parentTransform, Camera* camera)
{
	for (ContentList::const_iterator ci = content.begin();
		ci != content.end();
		ci++)
	{
		(*ci)->draw(parentTransform, camera);
	}
}