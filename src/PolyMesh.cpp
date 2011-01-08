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

#include <vector>

using namespace kocmoc;

PolyMesh::PolyMesh(unsigned int _primitiveCount, unsigned int _vertexIndexCount, unsigned int _vertexCount) : 
		primitiveCount(_primitiveCount),
		vertexIndexCount(_vertexIndexCount),
		vertexCount(_vertexCount),
		triangulatedVertexIndexCount(0),
		modelMatrix(glm::mat4(1.0f)),
		firstIndexArray(NULL),
		vertexIndexArray(NULL),
		uvIndexArray(NULL),
		vertexPositions(NULL),
		uvPositions(NULL),
		normalIndexArray(NULL),
		normalPositions(NULL),
		dataIsUploaded(false)
{}

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
	// TODO: clean up all intermediate arrays after upload


	// reindex data and convert to float
	// brute force implementation, i.e 1, 2, 3, .., n

	// expand to independent vertices
	double *reindexedVertexPositions = new double[vertexIndexCount*3];
	unsigned int *reindexedIndices = new unsigned int[vertexIndexCount];
	for (unsigned int i = 0; i < vertexIndexCount; i++)
	{
		reindexedVertexPositions[i*3] = vertexPositions[vertexIndexArray[i]*3];
		reindexedVertexPositions[i*3+1] = vertexPositions[vertexIndexArray[i]*3+1];
		reindexedVertexPositions[i*3+2] = vertexPositions[vertexIndexArray[i]*3+2];
		
		reindexedIndices[i] = i;
	}

	

	glGenVertexArrays(1, &vaoHandle);
	vboHandles = new GLuint[1];
	GLuint indicesHandle;
	glGenBuffers(1, vboHandles);
	glBindVertexArray(vaoHandle);
	

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexIndexCount * 3 *sizeof(double), reindexedVertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);


	//// normal
	//if (normalPositions != NULL)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
	//	glBufferData(GL_ARRAY_BUFFER, reindexedCount * 3 *sizeof(float), reindexedNormalPositions, GL_STATIC_DRAW);
	//	glVertexAttribPointer(VERTEX_ATTR_INDEX_NORMAL, 3, GL_DOUBLE, GL_FALSE, 0, 0);
	//	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_NORMAL);
	//}

	//// uv
	//if (uvPositions != NULL)
	//{
	//	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
	//	glBufferData(GL_ARRAY_BUFFER, reindexedCount * 2 *sizeof(float), reindexedUVPositions, GL_STATIC_DRAW);
	//	glVertexAttribPointer(VERTEX_ATTR_INDEX_UV0, 2, GL_DOUBLE, GL_FALSE, 0, 0);
	//	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);
	//}

	

	// indices
	// recalculate, i.e. triangulate
	std::vector<unsigned int> triangulatedIndices;

	for (unsigned int i = 0; i < primitiveCount; i++)
	{
		unsigned int count = firstIndexArray[i+1] - firstIndexArray[i];

		if (count == 3)
		{
			// just upload
			triangulatedIndices.push_back(reindexedIndices[firstIndexArray[i]]);
			triangulatedIndices.push_back(reindexedIndices[firstIndexArray[i]+1]);
			triangulatedIndices.push_back(reindexedIndices[firstIndexArray[i]+2]);
		}
		else if (count > 3)
		{
			// triangulate
			unsigned int firstIndex = firstIndexArray[i];
			for (unsigned int offset = 1; offset < (count - 1); offset++)
			{
				triangulatedIndices.push_back(reindexedIndices[firstIndex]);
				triangulatedIndices.push_back(reindexedIndices[firstIndex + offset]);
				triangulatedIndices.push_back(reindexedIndices[firstIndex + offset + 1]);
			}
		}
	}
	triangulatedVertexIndexCount = triangulatedIndices.size();
	
	// upload
	glGenBuffers(1, &indicesHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangulatedVertexIndexCount * sizeof(unsigned int), &triangulatedIndices[0], GL_STATIC_DRAW);


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
		cout << "failed to retrieve the uniform position for sTex0" << endl;
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
		glDrawElements(GL_TRIANGLES, triangulatedVertexIndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	shader->unbind();
}

void PolyMesh::setModelMatrix(glm::mat4 _modelMatrix)
{
	modelMatrix = _modelMatrix;
}

void PolyMesh::setUVPositions(double *uv)
{
	uvPositions = uv;
}

void PolyMesh::setUVIndexArray(unsigned int *indices)
{
	uvIndexArray = indices;
}

void PolyMesh::setNormalIndexArray(unsigned int *indices)
{
	normalIndexArray = indices;
}

void PolyMesh::setNormalPositions(double * normals)
{
	normalPositions = normals;
}