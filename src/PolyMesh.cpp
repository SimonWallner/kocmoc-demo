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

	glBindVertexArray(*vao_id);
get_errors();
	glGenBuffers(1, &vbo_id);
get_errors();
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(float), vertexPositions, GL_STATIC_DRAW);
get_errors();
	GLint vertex_location = shader.get_attrib_location("inPosition");
	glEnableVertexAttribArray(vertex_location);
get_errors();
	glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, NULL);
get_errors();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	get_errors();
}