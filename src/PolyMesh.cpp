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

void PolyMesh::setUV0(float * uv)
{
	delete [] vertexUVs;
	vertexUVs = uv;
}

void PolyMesh::setVertexNormals(float * normals)
{
	delete [] vertexNormals;
	vertexNormals = normals;
}

void PolyMesh::transferData(Shader *shader, GLuint *vao_id)
{
	vbo_id = new GLuint[3];
	glGenBuffers(3, vbo_id);

	glBindVertexArray(*vao_id);
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 *sizeof(float), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_UV0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

unsigned int PolyMesh::getVertexCount()
{
	return vertexCount;
}