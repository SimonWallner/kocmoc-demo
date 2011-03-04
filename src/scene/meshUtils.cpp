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

using std::vector;

SplitResult kocmoc::scene::meshUtils::splitMesh(const PolyMesh* mesh, const double d, const dvec3 n)
{
	if (mesh == NULL)
	{
		SplitResult result = {NULL, NULL};
		return result;
	}

	AttributeBufferMap insideAttributeBufferMap;
	AttributeBufferMap outsideAttributeBufferMap;

	for (PolyMesh::VertexAttributeMap::const_iterator ci = mesh->vertexAttributes.begin();
		ci != mesh->vertexAttributes.end();
		ci++)
	{
		insideAttributeBufferMap[ci->first] = AttributeBuffer();
		outsideAttributeBufferMap[ci->first] = AttributeBuffer();
	}

	uint insideFirstIndex = 0;
	uint oldInsideFirstIndex = 0;
	vector<uint > insideFirstIndexVector;
	insideFirstIndexVector.push_back(0);

	uint outsideFirstIndex = 0;
	uint oldOutsideFirstIndex = 0;
	vector<uint > outsideFirstIndexVector;
	outsideFirstIndexVector.push_back(0);

	PolyMesh::VertexAttribute position = mesh->vertexAttributes.find(symbolize("position"))->second;

	// for each primitive
	for (uint i = 0; i < mesh->primitiveCount; i++)
	{
		uint firstIndex = mesh->firstIndexArray[i];
		uint primitiveLength = mesh->firstIndexArray[i+1] - firstIndex;
		
		// for each vertex
		for (uint j = 0; j < primitiveLength; j++)
		{
			uint currentIndex = position.indices[firstIndex + j];
			uint nextIndex = position.indices[firstIndex + ((j+1) % primitiveLength)];

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
					transferAttributes(insideAttributeBufferMap, mesh->vertexAttributes, currentIndex);
					insideFirstIndex++;
				}
				else // inside-outside
				{
					transferAttributes(insideAttributeBufferMap, mesh->vertexAttributes, currentIndex);
					insideFirstIndex++;

					double r = intersect(d, n, p1, p2);
					lerpTransferAttributes(insideAttributeBufferMap,
						outsideAttributeBufferMap, mesh->vertexAttributes,
						currentIndex, nextIndex, r);
					insideFirstIndex++;
					outsideFirstIndex++;
				}
			}
			else // outside
			{
				if (inside(d, n, p2)) // outside-inside
				{
					transferAttributes(outsideAttributeBufferMap, mesh->vertexAttributes, currentIndex);
					outsideFirstIndex++;

					double r = intersect(d, n, p1, p2);
					lerpTransferAttributes(insideAttributeBufferMap,
						outsideAttributeBufferMap, mesh->vertexAttributes,
						currentIndex, nextIndex, r);
					insideFirstIndex++;
					outsideFirstIndex++;
				}
				else // outside-outside
				{
					transferAttributes(outsideAttributeBufferMap, mesh->vertexAttributes, currentIndex);
					outsideFirstIndex++;
				}
			}
		}

		if (oldInsideFirstIndex < insideFirstIndex) // vertices added!
		{
			insideFirstIndexVector.push_back(insideFirstIndex);
			oldInsideFirstIndex = insideFirstIndex;
		}
		if (oldOutsideFirstIndex < outsideFirstIndex) // vertices added!
		{
			outsideFirstIndexVector.push_back(outsideFirstIndex);
			oldOutsideFirstIndex = outsideFirstIndex;
		}
	}

	SplitResult result = {NULL, NULL};

	// inside
	if (insideFirstIndexVector.size() > 1)
	{
		uint* insideFirstIndexArray = new uint[insideFirstIndexVector.size()];
		for (uint i = 0; i < insideFirstIndexVector.size(); i++)
			insideFirstIndexArray[i] = insideFirstIndexVector[i];

		PolyMesh* insideMesh = new PolyMesh(insideFirstIndexVector.size() - 1, insideFirstIndex, insideFirstIndexArray);
		// for each attribute
		for (PolyMesh::VertexAttributeMap::const_iterator ci = mesh->vertexAttributes.begin();
		ci != mesh->vertexAttributes.end();
		ci++)
		{
			Symbol attributeName = ci->first;
			uint stride = ci->second.stride;

			AttributeBuffer sourceBuffer = insideAttributeBufferMap[attributeName];

			double* data = new double[sourceBuffer.nextIndex * stride];
			for (uint i = 0; i < sourceBuffer.nextIndex * stride; i++)
				data[i] = sourceBuffer.data[i];

			uint* indices = new uint[insideFirstIndex];
			for (uint i = 0; i < insideFirstIndex; i++)
				indices[i] = sourceBuffer.indices[i];

			PolyMesh::VertexAttribute attribute(stride, sourceBuffer.nextIndex * stride, data, indices, true);
			insideMesh->addVertexAttribute(attributeName, attribute);
		}
	}



	// outside
	if (outsideFirstIndexVector.size() > 1)
	{
		uint* outsideFirstIndexArray = new uint[outsideFirstIndexVector.size()];
		for (uint i = 0; i < outsideFirstIndexVector.size(); i++)
			outsideFirstIndexArray[i] = outsideFirstIndexVector[i];

		PolyMesh* outsideMesh = new PolyMesh(outsideFirstIndexVector.size() - 1, outsideFirstIndex, outsideFirstIndexArray);
		// for each attribute
		for (PolyMesh::VertexAttributeMap::const_iterator ci = mesh->vertexAttributes.begin();
		ci != mesh->vertexAttributes.end();
		ci++)
		{
			Symbol attributeName = ci->first;
			uint stride = ci->second.stride;

			AttributeBuffer sourceBuffer = outsideAttributeBufferMap[attributeName];

			double* data = new double[sourceBuffer.nextIndex * stride];
			for (uint i = 0; i < sourceBuffer.nextIndex * stride; i++)
				data[i] = sourceBuffer.data[i];

			uint* indices = new uint[outsideFirstIndex];
			for (uint i = 0; i < outsideFirstIndex; i++)
				indices[i] = sourceBuffer.indices[i];

			PolyMesh::VertexAttribute attribute(stride, sourceBuffer.nextIndex * stride, data, indices, true);
			outsideMesh->addVertexAttribute(attributeName, attribute);
		}
	}

	return result;
}

void kocmoc::scene::meshUtils::transferAttributes(AttributeBufferMap& targetBufferMap,
	const PolyMesh::VertexAttributeMap& vertexAttributes, const uint currentIndex)
{

	// for each attribute
	for (PolyMesh::VertexAttributeMap::const_iterator ci = vertexAttributes.begin();
		ci != vertexAttributes.end();
		ci++)
	{
		// add current point, i.e. p1
		PolyMesh::VertexAttribute sourceAttribute = ci->second;
		AttributeBuffer& targetBuffer = targetBufferMap[ci->first];

		if (targetBuffer.indexMap.find(sourceAttribute.indices[currentIndex]) == targetBuffer.indexMap.end())
		{
			// write index
			targetBuffer.indices.push_back(targetBuffer.nextIndex);
			// write mapping
			targetBuffer.indexMap[currentIndex] = targetBuffer.nextIndex;
			// write attribute data
			for (uint k = 0; k < sourceAttribute.stride; k++)
			{
				targetBuffer.data.push_back(sourceAttribute.attributeData[currentIndex * sourceAttribute.stride + k]);
			}
			// increment index
			targetBuffer.nextIndex++;
		}
		else
		{
			targetBuffer.indices.push_back(targetBuffer.indexMap[currentIndex]);
		}
	}
}

void kocmoc::scene::meshUtils::lerpTransferAttributes(AttributeBufferMap& insideTargetBufferMap,
	AttributeBufferMap& outsideTargetBufferMap,
	const PolyMesh::VertexAttributeMap& vertexAttributes,
	const uint currentIndex,
	const uint nextIndex,
	const double r)
{
	// for each attribute
	for (PolyMesh::VertexAttributeMap::const_iterator ci = vertexAttributes.begin();
		ci != vertexAttributes.end();
		ci++)
	{
		PolyMesh::VertexAttribute sourceAttribute = ci->second;
		AttributeBuffer& insideTargetBuffer = insideTargetBufferMap[ci->first];
		AttributeBuffer& outsideTargetBuffer = insideTargetBufferMap[ci->first];

		// add nu index
		insideTargetBuffer.indices.push_back(insideTargetBuffer.nextIndex);
		outsideTargetBuffer.indices.push_back(outsideTargetBuffer.nextIndex);

		// lerp and write data
		for (uint k = 0; k < sourceAttribute.stride; k++)
		{
			double result = lerp(r,
				sourceAttribute.attributeData[currentIndex*sourceAttribute.stride + k],
				sourceAttribute.attributeData[nextIndex*sourceAttribute.stride + k]);

			insideTargetBuffer.data.push_back(result);
			outsideTargetBuffer.data.push_back(result);
		}
	}
}