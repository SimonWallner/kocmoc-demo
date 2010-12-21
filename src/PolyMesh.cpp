/*
 * PolyMesh.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "PolyMesh.hpp"
#include "Kocmoc.hpp"
#include "Camera.hpp"
#include "utility.hpp"

using namespace kocmoc;

PolyMesh::PolyMesh(unsigned int _primitiveCount, unsigned int _vertexIndexCount, unsigned int _vertexCount) : 
		primitiveCount(_primitiveCount),
		vertexIndexCount(_vertexIndexCount),
		vertexCount(_vertexCount)
{
	firstIndexArray = NULL;
	vertexIndexArray = NULL;
	vertexPositions = NULL;

	dataIsUploaded = false;
//	renderMesh = NULL;
}

PolyMesh::~PolyMesh()
{
	// TODO: clean up!
}


void PolyMesh::setVertexPositions(double * p)
{
	util::safeDeleteArray(vertexPositions);
	vertexPositions = p;
}

void PolyMesh::setVertexIndexArray(unsigned int *indices)
{
	util::safeDeleteArray(vertexIndexArray);
	vertexIndexArray = indices;
}

void PolyMesh::setFirstIndexArray(unsigned int *fia)
{
	util::safeDeleteArray(firstIndexArray);
	firstIndexArray = fia;
}


void PolyMesh::transferData()
{
	glGenVertexArrays(1, &vaoHandle);

	vboHandles = new GLuint[3];
	GLuint indicesHandle;
	glGenBuffers(3, vboHandles);

	glBindVertexArray(vaoHandle);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 *sizeof(double), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_DOUBLE, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);

	// indices
	glGenBuffers(1, &indicesHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndexCount * sizeof(unsigned int), vertexIndexArray, GL_STATIC_DRAW);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	dataIsUploaded = true;
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
		glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	shader->unbind();
}

