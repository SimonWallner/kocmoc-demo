/*
 * PolyMesh.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "PolyMesh.hpp"

PolyMesh::PolyMesh(int _vertexCount) : vertexCount(_vertexCount)
{
	vertexPositions = new float[vertexCount * 3];
	vertexNormals = new float[vertexCount * 3];
	vertexUVs = new float[vertexCount * 2];
}

PolyMesh::~PolyMesh()
{
	delete [] vertexPositions;
	delete [] vertexNormals;
	delete [] vertexUVs;
}


void PolyMesh::setVertexPositions(float * p)
{
	delete [] vertexPositions;
	vertexPositions = p;
}