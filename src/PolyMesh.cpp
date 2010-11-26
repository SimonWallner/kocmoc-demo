/*
 * PolyMesh.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "PolyMesh.hpp"

PolyMesh::PolyMesh(int _vertexCount) : vertexCount(_vertexCount)
{
	vertexPositions = new float[vertexCount];
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

void PolyMesh::transferData(Shader *shader, GLuint *vao_id)
{
	glBindVertexArray(*vao_id);
	glGenBuffers(1, &vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(float), vertexPositions, GL_STATIC_DRAW);

	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}