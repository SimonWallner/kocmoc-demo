#include "Octree.hpp"

#include <util/util.hpp>
#include <camera/Camera.hpp>
#include <renderer/RenderMesh.hpp>
#include <renderer/Shader.hpp>
#include <scene/meshUtils.hpp>
#include <Kocmoc.hpp>

using namespace kocmoc::scene;

using kocmoc::util::generator::generateOriginGizmo;
using kocmoc::util::generator::generateUnitCube;
using kocmoc::camera::Camera;
using kocmoc::renderer::RenderMesh;
using kocmoc::renderer::Shader;
using kocmoc::scene::PolyMesh;
using kocmoc::scene::meshUtils::SplitResult;
using kocmoc::scene::meshUtils::splitMesh;

using glm::vec3;
using glm::vec4;
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
	boundingBox = generateUnitCube(vec3(1.0f, 1.0f, 0.0f));
	originGizmo = generateOriginGizmo();
}


void Octree::insert(RenderMesh* mesh, int maxRecursionDepth)
{
	if (mesh == NULL) 
		return;

	if (maxRecursionDepth == 0)
	{
		content.push_back(mesh);
		return;
	}

	std::cout << ".";

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
				insert(content[i], maxRecursionDepth);

			content.clear();
		}
	}
	else
	{
		vec3 distance = vec3(0) - origin;
		Shader* shader = mesh->shader;
		
		// X
		SplitResult resultX = splitMesh(mesh->mesh, distance.x, dvec3(-1, 0, 0));
		PolyMesh* inside = resultX.inside;
		PolyMesh* outside = resultX.outside;
		delete mesh;

		// Y
		SplitResult insideY = splitMesh(inside, distance.y, dvec3(0, -1, 0));
		PolyMesh* insideInside = insideY.inside;
		PolyMesh* insideOutside = insideY.outside;
		delete inside;

		SplitResult outsideY = splitMesh(outside, distance.y, dvec3(0, -1, 0));
		PolyMesh* outsideInside = outsideY.inside;
		PolyMesh* outsideOutside = outsideY.outside;
		delete outside;

		// Z
		SplitResult insideInsideZ = splitMesh(insideInside, distance.z, dvec3(0, 0, -1));
		PolyMesh* iii = insideInsideZ.inside;
		PolyMesh* iio = insideInsideZ.outside;
		delete insideInside;

		SplitResult insideOutsideZ = splitMesh(insideOutside, distance.z, dvec3(0, 0, -1));
		PolyMesh* ioi = insideOutsideZ.inside;
		PolyMesh* ioo = insideOutsideZ.outside;
		delete insideOutside;

		SplitResult outsideInsideZ = splitMesh(outsideInside, distance.z, dvec3(0, 0, -1));
		PolyMesh* oii = outsideInsideZ.inside;
		PolyMesh* oio = outsideInsideZ.outside;
		delete outsideInside;

		SplitResult outsideOutsideZ = splitMesh(outsideOutside, distance.z, dvec3(0, 0, -1));
		PolyMesh* ooi = outsideOutsideZ.inside;
		PolyMesh* ooo = outsideOutsideZ.outside;
		delete outsideOutside;

		
		if (iii != NULL)
			children[0]->insert(new RenderMesh(iii, shader), maxRecursionDepth-1);
		if (oii != NULL)
			children[1]->insert(new RenderMesh(oii, shader), maxRecursionDepth-1);
		if (ooi != NULL)
			children[2]->insert(new RenderMesh(ooi, shader), maxRecursionDepth-1);
		if (ioi != NULL)
			children[3]->insert(new RenderMesh(ioi, shader), maxRecursionDepth-1);
		if (iio != NULL)
			children[4]->insert(new RenderMesh(iio, shader), maxRecursionDepth-1);
		if (oio != NULL)
			children[5]->insert(new RenderMesh(oio, shader), maxRecursionDepth-1);
		if (ooo != NULL)
			children[6]->insert(new RenderMesh(ooo, shader), maxRecursionDepth-1);
		if (ioo != NULL)
			children[7]->insert(new RenderMesh(ioo, shader), maxRecursionDepth-1);
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

void Octree::draw(mat4 parentTransform, Camera* camera, bool cull)
{
	uint visibleSamples = 0;
	mat4 viewProjection = camera->getProjectionMatrix() * camera->getViewMatrix();

	visibleSamples += isVisible(origin + vec3(size), viewProjection);
	visibleSamples += isVisible(origin + vec3(size, size, -size), viewProjection);
	visibleSamples += isVisible(origin + vec3(size, -size, size), viewProjection);
	visibleSamples += isVisible(origin + vec3(size, -size, -size), viewProjection);
	visibleSamples += isVisible(origin + vec3(-size, size, size), viewProjection);
	visibleSamples += isVisible(origin + vec3(-size, size, -size), viewProjection);
	visibleSamples += isVisible(origin + vec3(-size, -size, size), viewProjection);
	visibleSamples += isVisible(origin + vec3(-size, -size, -size), viewProjection);


	if (!(Kocmoc::paramMapBool[symbolize("viewFrustumCulling")] && cull) || visibleSamples > 0) // (VFC && cull) implies visibility
	{
		if (!isLeaf)
		{
			for (uint i = 0; i < 8; i++)
				children[i]->draw(parentTransform, camera);
		}
		else 
		{
			for (ContentList::const_iterator ci = content.begin();
				ci != content.end();
				ci++)
			{
				(*ci)->draw(parentTransform, camera);
			}
		}
	}
}

bool Octree::isVisible(vec3 sample, mat4 viewProjection)
{
	vec4 projectedSample = viewProjection * vec4(sample, 1);
	projectedSample = projectedSample / projectedSample.w;

	return (projectedSample.x > -1 && projectedSample.y > -1 && projectedSample.z > -1
		&& projectedSample.x < 1 && projectedSample.y < 1 && projectedSample.z < 1);
}