#include "PolyMesh.hpp"
#include "Symbol.hpp"

using namespace kocmoc::scene;

using kocmoc::symbolize;
using kocmoc::Symbol;

using glm::dvec3;


PolyMesh::PolyMesh(uint _primitiveCount, uint _vertexIndexCount,
	uint* _firstIndexArray, VertexAttribute vertexPositions)
	: primitiveCount(_primitiveCount)
	, vertexIndexCount(_vertexIndexCount)
	, firstIndexArray(_firstIndexArray)
{
	vertexAttributes.insert(VertexAttributePair(symbolize("position"), vertexPositions));
}

PolyMesh::~PolyMesh()
{
	for (VertexAttributeMap::const_iterator ci = vertexAttributes.begin();
		ci != vertexAttributes.end();
		ci++)
	{
		if ((*ci).second.hasOwnership)
		{
			delete[] (*ci).second.attributeData;
			delete[] (*ci).second.indices;
		}
	}
}

void PolyMesh::addVertexAttribute(Symbol name, VertexAttribute attribute)
{
	vertexAttributes.insert(VertexAttributePair(name, attribute));
}

void PolyMesh::addTexture(Symbol name, Texture texture)
{
	textures.insert(TexturePair(name, texture));
}

PolyMesh::BoundingBox PolyMesh::calculateBoundingBox() const
{
	double minX, minY, minZ;
	minX = minY = minZ = std::numeric_limits<double>::max();
	double maxX, maxY, maxZ;
	maxX = maxY = maxZ = -std::numeric_limits<double>::max();

	VertexAttributeMap::const_iterator ci = vertexAttributes.find(symbolize("position"));
	const double* data = ci->second.attributeData;

	for (uint i= 0; i < ci->second.attributeDataLength/ci->second.stride; i++)
	{
		if (data[i*3  ] < minX) minX = data[i*3  ];
		if (data[i*3+1] < minY) minY = data[i*3+1];
		if (data[i*3+2] < minZ) minZ = data[i*3+2];

		if (data[i*3  ] > maxX) maxX = data[i*3  ];
		if (data[i*3+1] > maxY) maxY = data[i*3+1];
		if (data[i*3+2] > maxZ) maxZ = data[i*3+2];
	}
	BoundingBox bb;
	bb.min = dvec3(minX, minY, minZ);
	bb.max = dvec3(maxX, maxY, maxZ);
	return bb;
}