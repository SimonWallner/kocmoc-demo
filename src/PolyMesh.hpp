/*
 * PolyMesh.hpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#ifndef POLYMESH_HPP_
#define POLYMESH_HPP_

#include "Shader.hpp"

class PolyMesh
{
public:
	PolyMesh(int vertexCount);
	virtual ~PolyMesh();
	void setVertexPositions(float * p);
	
	/**
	 * Transfer the data to the gpu, i.e. write the vbos, etc...
	 */
	void transferData(Shader *shader, GLuint * vao_id);



private:
	const unsigned int vertexCount;
	float * vertexPositions;
	float * vertexNormals;
	float * vertexUVs;
	
	GLuint vbo_id;
};

#endif /* POLYMESH_HPP_ */
