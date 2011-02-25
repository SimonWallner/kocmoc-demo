#include "meshUtils.hpp"

#include <scene/PolyMesh.hpp>
#include <util/util.hpp>

#include <glm.hpp>

using namespace kocmoc::scene::meshUtils;
using kocmoc::util::geometry::inside;
using kocmoc::util::geometry::intersect;
using kocmoc::util::geometry::lerp;

using glm::dvec3;


using glm::vec3;

SplitResult kocmoc::scene::meshUtils::splitMesh(const PolyMesh* mesh, const double d, const vec3 n)
{
	//vector<vec3> positionsInside;
	//vector<vec3> positionsOutside;

	//vector<vec3> uvInside;
	//vector<vec3> uvOutside;

	//vector<vec3> nInside;
	//vector<vec3> nOutside;

	//vector<uint> firstIndexInside;
	//vector<uint> firstIndexOutside;

	//firstIndexInside.push_back(0);
	//firstIndexOutside.push_back(0);

	//uint fiInside = 0;
	//uint oldfiInside = 0;
	//uint fiOutside = 0;
	//uint oldfiOutside = 0;

	//uint primitiveCountInside = 0;
	//uint primitiveCountOutside = 0;

	AttributeBufferMap insideAttributeBufferMap;
	AttributeBufferMap outsideAttributeBufferMap;
	for (PolyMesh::VertexAttributeMap.const_iterator ci = mesh->vertexAttributes.begin();
		ci != mesh->vertexAttributes.end();
		ci++)
	{
		insideAttributeBufferMap[ci->first] = AttributeBuffer();
		outsideAttributeBufferMap[ci->first] = AttributeBuffer();
	}

	PolyMesh::VertexAttribute position = mesh->vertexAttributes.find(symbolize("position"))->second;

	// for each primitive
	for (uint i = 0; i < mesh->primitiveCount; i++)
	{
		uint firstIndex = mesh->firstIndexArray[i];
		uint primitiveLength = mesh->firstIndexArray[i+1] - firstIndex;
		
		// for each vertex
		for (uint j = 0; j < primitiveLength; j++)
		{
			uint currentIndex = firstIndex + j;
			uint nextIndex = firstIndex + ((j+1) % primitiveLength);

			dvec3 p1 = dvec3(position.attributeData[currentIndex*3],
							position.attributeData[currentIndex*3 + 1],
							position.attributeData[currentIndex*3 + 2]);

			dvec3 p2 = dvec3(position.attributeData[nextIndex*3],
							position.attributeData[nextIndex*3 + 1],
							position.attributeData[nextIndex*3 + 2]);

			if (inside(d, n, p1))
			{
				if (inside(d, n, p2)) // inside-inside
				{
					// for each attribute
					for (PolyMesh::VertexAttributeMap::const_iterator ci = mesh->vertexAttributes.begin();
						ci != mesh->vertexAttributes.end();
						ci++)
					{
						// add current point, i.e. p1
						//...

					//positionsInside.push_back(p1);
					//uvInside.push_back(uv1);
					//nInside.push_back(n1);
					//fiInside++;
				
				}
				else // inside-outside
				{
					double r = intersect(d, n, p1, p2);
					vec3 iP = lerp(r, p1, p2);
					//vec3 iUV = lerp(r, uv1, uv2);
					//vec3 iN = lerp(r, n1, n2);

					positionsInside.push_back(p1);
					//uvInside.push_back(uv1);
					//nInside.push_back(n1);
					fiInside++;
				
					positionsInside.push_back(iP);
					//uvInside.push_back(iUV);
					//nInside.push_back(iN);
					fiInside++;

					positionsOutside.push_back(iP);
					//uvOutside.push_back(iUV);
					//nOutside.push_back(iN);
					fiOutside++;

				}
			}
			else // outside
			{
				if (inside(d, n, p2)) // outside-inside
				{
					double r = intersect(d, n, p1, p2);
					vec3 iP = lerp(r, p1, p2);
					//vec3 iUV = lerp(r, uv1, uv2);
					//vec3 iN = lerp(r, n1, n2);

					positionsInside.push_back(iP);
					//uvInside.push_back(iUV);
					//nInside.push_back(iN);
					fiInside++;
					
					positionsOutside.push_back(p1);
					//uvOutside.push_back(uv1);
					//nOutside.push_back(n1);
					fiOutside++;

					positionsOutside.push_back(iP);
					//uvOutside.push_back(iUV);
					//nOutside.push_back(iN);
					fiOutside++;
				}
				else // outside-outside
				{
					positionsOutside.push_back(p1);
					//uvOutside.push_back(uv1);
					//nOutside.push_back(n1);
					fiOutside++;
				}
			}
		}

		if (oldfiInside < fiInside) // vertices added!
		{
			firstIndexInside.push_back(fiInside);
			primitiveCountInside++;
			oldfiInside = fiInside;
		}
		if (oldfiOutside < fiOutside) // vertices added!
		{
			firstIndexOutside.push_back(fiOutside);
			primitiveCountOutside++;
			oldfiOutside = fiOutside;
		}
	}

	SplitResult result;

	// inside
	result.inside = new PolyMesh(primitiveCountInside, positionsInside.size(), positionsInside.size() * 3);
	double* iPositions = new double[positionsInside.size()];
	//double* iUV = new double[positionsInside.size()];
	//double* iNormals = new double[positionsInside.size()];
	uint* iIndices = new uint[positionsInside.size()];

	for (uint i = 0; i < positionsInside.size(); i++)
	{
		vec3 p = positionsInside[i];
		//vec3 uv = uvInside[i];
		//vec3 n = nInside[i];

		iPositions[i*3] = p.x;
		iPositions[i*3+1] = p.y;
		iPositions[i*3+2] = p.z;

		//iUV[i] = uv.x;
		//iUV[i] = uv.y;
		//iUV[i] = uv.z;

		//iNormals[i] = n.x;
		//iNormals[i] = n.y;
		//iNormals[i] = n.z;

		iIndices[i] = i; // brute force indexing !!!
	}
	result.inside->setFirstIndexArray(&firstIndexInside[0]);
	result.inside->setVertexPositions(iPositions);
	result.inside->setVertexIndexArray(iIndices);
	//result.inside->setNormalIndexArray(iFia);
	//result.inside->setNormalPositions(iNormals);
	//result.inside->setUVIndexArray(iFia);
	//result.inside->setUVPositions(iUV);
	//result.inside->setNormalPositions(iNormals);

	result.inside->setDiffuseTexture(diffuseTextureHandle);
	result.inside->setSpecularTexture(specularTextureHandle);
	result.inside->setNormalTexture(normalTextureHandle);
	result.inside->setShader(shader);


	// outside
	result.outside = new PolyMesh(primitiveCountOutside, positionsOutside.size(), positionsOutside.size() * 3);
	double* oPositions = new double[positionsOutside.size()];
	//double* oUV = new double[positionsOutside.size()];
	//double* oNormals = new double[positionsOutside.size()];
	uint* oIndices = new uint[positionsOutside.size()];

	for (uint i = 0; i < positionsOutside.size(); i++)
	{
		vec3 p = positionsOutside[i];
		//vec3 uv = uvOutside[i];
		//vec3 n = nOutside[i];

		oPositions[i*3] = p.x;
		oPositions[i*3+1] = p.y;
		oPositions[i*3+2] = p.z;

		//oUV[i] = uv.x;
		//oUV[i] = uv.y;
		//oUV[i] = uv.z;

		//oNormals[i] = n.x;
		//oNormals[i] = n.y;
		//oNormals[i] = n.z;

		oIndices[i] = i; // brute force indexing !!!
	}
	result.outside->setFirstIndexArray(&firstIndexOutside[0]);
	result.outside->setVertexPositions(oPositions);
	result.outside->setVertexIndexArray(oIndices);
	//result.outside->setNormalIndexArray(oFia);
	//result.outside->setNormalPositions(oNormals);
	//result.outside->setUVIndexArray(oFia);
	//result.outside->setUVPositions(oUV);
	//result.outside->setNormalPositions(oNormals);

	result.outside->setDiffuseTexture(diffuseTextureHandle);
	result.outside->setSpecularTexture(specularTextureHandle);
	result.outside->setNormalTexture(normalTextureHandle);
	result.outside->setShader(shader);

	return result;
}