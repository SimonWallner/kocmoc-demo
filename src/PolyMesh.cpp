/*
 * PolyMesh.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "PolyMesh.hpp"
#include "Kocmoc.hpp"
#include "Camera.hpp"

PolyMesh::PolyMesh(int _vertexCount) :
	vertexCount(_vertexCount), 
	dataIsUploaded(false),
	modelMatrix(mat4(1.0f))
{
	vertexPositions = new float[vertexCount * 3];
	vertexNormals = new float[vertexCount * 3];
	vertexUVs = new float[vertexCount * 2];
}

PolyMesh::~PolyMesh()
{
	/*delete [] vertexPositions;
	delete [] vertexNormals;
	delete [] vertexUVs;*/
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

void PolyMesh::transferData()
{
	glGenVertexArrays(1, &vaoHandle);

	vboHandles = new GLuint[3];
	glGenBuffers(3, vboHandles);

	glBindVertexArray(vaoHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 *sizeof(float), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), vertexUVs, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_UV0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), vertexNormals, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_NORMAL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	dataIsUploaded = true;
}

unsigned int PolyMesh::getVertexCount()
{
	return vertexCount;
}

void PolyMesh::setShader(Shader *_shader)
{
	shader = _shader;
}

void PolyMesh::setTexture(GLint _textureHandle)
{
	textureHandle = _textureHandle;

	GLint sTex0_location = shader->get_uniform_location("sTex0");
		
	if (sTex0_location >= 0)
		glUniform1i(sTex0_location, 0);
	else 
	{
		cout << "failed to get retrieve the uniform position for sTex0" << endl;
		cout << "uniform locatio is: " << sTex0_location << endl;
	}
}

void PolyMesh::draw()
{
	if (!dataIsUploaded)
		transferData();

	Camera *camera = Kocmoc::getInstance().getCamera();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureHandle);

	shader->bind();
	{
		GLint projectionMatrix_location = shader->get_uniform_location("projectionMatrix");		glUniformMatrix4fv(projectionMatrix_location, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
		GLint viewMatrix_location = shader->get_uniform_location("viewMatrix");		glUniformMatrix4fv(viewMatrix_location, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		GLint modelMatrix_location = shader->get_uniform_location("modelMatrix");		glUniformMatrix4fv(modelMatrix_location, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		glBindVertexArray(0);
	}
		shader->unbind();
}

