#ifndef POLYMESH_HPP_
#define POLYMESH_HPP_

#include "Shader.hpp"
#include "RenderMesh.hpp"

/**
 * The PolyMesh acts as the cannonical basis for meshes in this application.
 *
 * Data that is generated or imported is transfered into this internal
 * representation. It preserves the topological information on a low level basis.
 * That is, primitives with vertex indices in a set of shared vertices. Adjacency
 * information is not directly available but could be computed from the data.
 *
 * Data is organized in primitives (points, lines, convex n-gons), vertex indices
 * of this primitives and shared vertex attributes (position, etc...).
 */

// TODO: additional vertex attributes
// TODO: Face attributes
// TODO: Face-Vertex attributes

class PolyMesh
{
public:
	/**
	 * Create a new poly mesh. Counts must be known in advance to avoid 
	 * reallocation of arrays and such
	 */
	PolyMesh(unsigned int primitiveCount,
		unsigned int vertexIndexCount,
		unsigned int vertexCount);

	virtual ~PolyMesh();

	void setVertexIndexArray(unsigned int *vertexIndices);
	void setFirstIndexArray(unsigned int *fia);
	void setVertexPositions(double *positions);

	// TODO: set normals, uvs, color, etc...

	/**
	 * Draw the mesh.
	 * If the data is not on the gpu yet it will be transfered upon the first call
	 */
	void draw();

	void setShader(Shader *shader);

	void setTexture(GLint textureHandle);

	// TODO: recompute face normals

	unsigned int getPrimitiveCount(void) {return primitiveCount;};
	unsigned int getVertexIndexCount(void) {return vertexIndexCount;};
	unsigned int getVertexCount(void) {return vertexCount;};

private:

	/**
	 * Transfer the data to the gpu, i.e. write the vbos, etc...
	 */
	// create and organize renderMesh thingys
	void transferData();

	/** total number of vertices **/
	unsigned int vertexCount;

	/** number of vertex indices **/
	unsigned int vertexIndexCount;

	/** number of primitives **/
	unsigned int primitiveCount;

	unsigned int *firstIndexArray;
	unsigned int *vertexIndexArray;
	double *vertexPositions;
	
	mat4 modelMatrix;

	/** the GPU view of the polyMesh **/
	//RenderMesh *renderMesh;

	// TODO: refactor into render mesh class
	// make a poly Mesh hold a render mesh for rendering
	GLuint vaoHandle, *vboHandles;

	GLuint textureHandle;
	Shader *shader;

	bool dataIsUploaded;
};

#endif /* POLYMESH_HPP_ */
