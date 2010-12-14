#ifndef POLYMESH_HPP_
#define POLYMESH_HPP_

#include "shader.hpp"

/**
 * The polyMesh represents the model data as it is used in the application domain
 * and it is also responsible for creating a 'view' of the model in the 
 * GPU domain, i.e. create the vertex arrays and transfer them to the GPU.
 */
class PolyMesh
{
public:
	PolyMesh(int vertexCount);
	virtual ~PolyMesh();
	void setVertexPositions(float * p);
	void setUV0(float * uv);
	void setVertexNormals(float* normals);
	
	unsigned int getVertexCount(void);

	/**
	 * Draw the mesh.
	 * If the data is not on the gpu yet it will be transfered upon the first call
	 */
	void draw();

	void setShader(Shader *shader);

	void setTexture(GLint textureHandle);

private:

	/**
	 * Transfer the data to the gpu, i.e. write the vbos, etc...
	 */
	void transferData();

	const unsigned int vertexCount;
	float * vertexPositions;
	float * vertexNormals;
	float * vertexUVs;
	
	GLuint vaoHandle, *vboHandles;

	GLuint textureHandle;
	Shader *shader;

	mat4 modelMatrix;

	bool dataIsUploaded;


};

#endif /* POLYMESH_HPP_ */
