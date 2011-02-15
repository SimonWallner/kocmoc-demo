/*
 * PolyMesh.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "PolyMesh.hpp"

#include <Kocmoc.hpp>
#include <util/util.hpp>

#include <gtx/inverse_transpose.hpp>

#include <vector>

using namespace kocmoc::scene;
using kocmoc::renderer::Shader;
using kocmoc::camera::Camera;
using glm::mat4;


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
		dataIsUploaded(false),
		diffuseTextureHandle(0),
		specularTextureHandle(0),
		normalTextureHandle(0)
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
	unsigned int *reindexedIndices = new unsigned int[vertexIndexCount];
	float *reindexedVertexPositions = new float[vertexIndexCount*3];
	float *reindexedNormalPositions = new float[vertexIndexCount*3];
	float *reindexedUVPositions = new float[vertexIndexCount*2];
	for (unsigned int i = 0; i < vertexIndexCount; i++)
	{
		reindexedVertexPositions[i*3] = vertexPositions[vertexIndexArray[i]*3];
		reindexedVertexPositions[i*3+1] = vertexPositions[vertexIndexArray[i]*3+1];
		reindexedVertexPositions[i*3+2] = vertexPositions[vertexIndexArray[i]*3+2];

		if (normalPositions != NULL)
		{
			reindexedNormalPositions[i*3] = normalPositions[normalIndexArray[i]*3];
			reindexedNormalPositions[i*3+1] = normalPositions[normalIndexArray[i]*3+1];
			reindexedNormalPositions[i*3+2] = normalPositions[normalIndexArray[i]*3+2];
		}

		if (uvPositions != NULL)
		{
			reindexedUVPositions[i*2] = uvPositions[uvIndexArray[i]*2];
			reindexedUVPositions[i*2+1] = uvPositions[uvIndexArray[i]*2+1];
		}

		reindexedIndices[i] = i;
	}

	

	glGenVertexArrays(1, &vaoHandle);
	vboHandles = new GLuint[3];
	GLuint indicesHandle;
	glGenBuffers(3, vboHandles);
	glBindVertexArray(vaoHandle);
	

	glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexIndexCount * 3 *sizeof(float), reindexedVertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(VERTEX_ATTR_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(VERTEX_ATTR_INDEX_POSITION);


	// normal
	if (normalPositions != NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[1]);
		glBufferData(GL_ARRAY_BUFFER, vertexIndexCount * 3 *sizeof(float), reindexedNormalPositions, GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_ATTR_INDEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_ATTR_INDEX_NORMAL);
	}

	// uv
	if (uvPositions != NULL)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[2]);
		glBufferData(GL_ARRAY_BUFFER, vertexIndexCount * 2 *sizeof(float), reindexedUVPositions, GL_STATIC_DRAW);
		glVertexAttribPointer(VERTEX_ATTR_INDEX_UV0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(VERTEX_ATTR_INDEX_UV0);
	}

	

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
	

	// create gizmo
	gizmo = util::generator::generateUnitCube();
	dataIsUploaded = true;
}

void PolyMesh::setShader(Shader *_shader)
{
	shader = _shader;
}

void PolyMesh::setDiffuseTexture(GLint _textureHandle)
{
	diffuseTextureHandle = _textureHandle;
}

void PolyMesh::setSpecularTexture(GLint _textureHandle)
{
	specularTextureHandle = _textureHandle;
}

void PolyMesh::setNormalTexture(GLint _textureHandle)
{
	normalTextureHandle = _textureHandle;
}

void PolyMesh::draw(glm::mat4 parentTransform, Camera *camera, Shader *shadowShader)
{
	Shader* activeShader;
	if (shadowShader == NULL)
		activeShader = shader;
	else 
		activeShader = shadowShader;

	glm::mat4 leafTransform = parentTransform * modelMatrix;
	glm::mat3 normalMatrix = glm::gtx::inverse_transpose::inverseTranspose(glm::mat3(leafTransform));

	if (!dataIsUploaded)
		transferData();


	if (diffuseTextureHandle > 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTextureHandle);
	}

	if (specularTextureHandle > 0)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTextureHandle);
	}

	if (normalTextureHandle > 0)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normalTextureHandle);
	}

	activeShader->bind();
	{

		GLint location;
		if ((location = activeShader->get_uniform_location("cameraPosition")) >= 0)
			glUniform3fv(location, 1, glm::value_ptr(camera->getPosition()));

		if ((location = activeShader->get_uniform_location("projectionMatrix")) >= 0)			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));
		if ((location = activeShader->get_uniform_location("viewMatrix")) >= 0)			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
		if ((location = activeShader->get_uniform_location("modelMatrix")) >= 0)			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(leafTransform));

		if ((location = activeShader->get_uniform_location("shadowPV")) >= 0)
		{
			mat4 shadowPV = Kocmoc::getInstance().getOrthoCamera()->getProjectionMatrix() * Kocmoc::getInstance().getOrthoCamera()->getViewMatrix();
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(shadowPV));
		}

		if (normalPositions != NULL && (location = activeShader->get_uniform_location("normalMatrix")) >= 0)
			glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(normalMatrix));	
	
		glBindVertexArray(vaoHandle);
		glDrawElements(GL_TRIANGLES, triangulatedVertexIndexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	activeShader->unbind();

	gizmo->draw(leafTransform, camera);
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