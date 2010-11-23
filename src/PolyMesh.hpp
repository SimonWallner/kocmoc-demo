/*
 * PolyMesh.hpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#ifndef POLYMESH_HPP_
#define POLYMESH_HPP_

class PolyMesh
{
public:
	PolyMesh(int vertexCount);
	virtual ~PolyMesh();
	void setVertexPositions(float * p);

private:
	int vertexCount;
	float * vertexPositions;
	float * vertexNormals;
	float * vertexUVs;
};

#endif /* POLYMESH_HPP_ */
