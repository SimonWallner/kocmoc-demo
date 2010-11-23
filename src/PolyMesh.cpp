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

void PolyMesh::transferData(Shader shader, GLuint *vao_id)
{
	glGenVertexArrays(1, vao_id);
	glBindVertexArray(*vao_id);

	glGenBuffers(1, &vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertexPositions, GL_STATIC_DRAW);

	GLint vertex_location = shader.get_attrib_location("inPosition");
	glEnableVertexAttribArray(vertex_location);
	glVertexAttribPointer(vertex_location, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}