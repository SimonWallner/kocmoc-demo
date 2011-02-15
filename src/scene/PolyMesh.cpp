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
using namespace kocmoc::util::geometry;

using kocmoc::renderer::Shader;
using kocmoc::camera::Camera;

using glm::mat4;
using glm::vec3;

using std::vector;


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

PolyMesh::SplitResult PolyMesh::split(double d, vec3 n)
{
	vector<vec3> positionsInside;
	vector<vec3> positionsOutside;

	vector<vec3> uvInside;
	vector<vec3> uvOutside;

	vector<vec3> nInside;
	vector<vec3> nOutside;

	vector<uint> firstIndexInside;
	vector<uint> firstIndexOutside;

	firstIndexInside.push_back(0);
	firstIndexOutside.push_back(0);

	uint fiInside = 0;
	uint oldfiInside = 0;
	uint fiOutside = 0;
	uint oldfiOutside = 0;

	uint primitiveCountInside = 0;
	uint primitiveCountOutside = 0;

	for (uint i = 0; i < primitiveCount; i++)
	{
		uint firstIndex = firstIndexArray[i];
		uint length = firstIndexArray[i+1] - firstIndex;

		for (uint j = 0; j < length; j += 3)
		{
			vec3 p1 = vec3(vertexPositions[firstIndex + (j % length)],
							vertexPositions[firstIndex + (j+1 % length)],
							vertexPositions[firstIndex + (j+2 % length)]);

			vec3 p2 = vec3(vertexPositions[firstIndex + (j+3 % length)],
							vertexPositions[firstIndex + (j+4 % length)],
							vertexPositions[firstIndex + (j+5 % length)]);

			//vec3 uv1 = vec3(uvPositions[firstIndex + (j % length)],
			//				uvPositions[firstIndex + (j+1 % length)],
			//				uvPositions[firstIndex + (j+2 % length)]);

			//vec3 uv2 = vec3(uvPositions[firstIndex + (j+3 % length)],
			//				uvPositions[firstIndex + (j+4 % length)],
			//				uvPositions[firstIndex + (j+5 % length)]);

			//vec3 n1 = vec3(normalPositions[firstIndex + (j % length)],
			//				normalPositions[firstIndex + (j+1 % length)],
			//				normalPositions[firstIndex + (j+2 % length)]);
			//
			//vec3 n2 = vec3(normalPositions[firstIndex + (j+3 % length)],
			//				normalPositions[firstIndex + (j+4 % length)],
			//				normalPositions[firstIndex + (j+5 % length)]);

			if (inside(d, n, p1))
			{
				if (inside(d, n, p2)) // inside-inside
				{
					positionsInside.push_back(p1);
					//uvInside.push_back(uv1);
					//nInside.push_back(n1);
					fiInside++;
				
				}
				else // inside-outside
				{
					double r = intersect(d, n, p1, p2);
					vec3 iP = mix(r, p1, p2);
					//vec3 iUV = mix(r, uv1, uv2);
					//vec3 iN = mix(r, n1, n2);

					positionsInside.push_back(p1);
					//uvInside.push_back(uv1);
					//nInside.push_back(n1);
					fiInside++;
				
					positionsInside.push_back(iP);
					//uvInside.push_back(iUV);
					//nInside.push_back(iN);
					fiInside++;

					positionsOutside.push_back(iP);
					//uvOutside.push_back(iUV);
					//nOutside.push_back(iN);
					fiOutside++;

				}
			}
			else // outside
			{
				if (inside(d, n, p2)) // outside-inside
				{
					double r = intersect(d, n, p1, p2);
					vec3 iP = mix(r, p1, p2);
					//vec3 iUV = mix(r, uv1, uv2);
					//vec3 iN = mix(r, n1, n2);

					positionsInside.push_back(iP);
					//uvInside.push_back(iUV);
					//nInside.push_back(iN);
					fiInside++;
					
					positionsOutside.push_back(p1);
					//uvOutside.push_back(uv1);
					//nOutside.push_back(n1);
					fiOutside++;

					positionsOutside.push_back(iP);
					//uvOutside.push_back(iUV);
					//nOutside.push_back(iN);
					fiOutside++;
				}
				else // outside-outside
				{
					positionsOutside.push_back(p1);
					//uvOutside.push_back(uv1);
					//nOutside.push_back(n1);
					fiOutside++;
				}
			}

			if (oldfiInside < fiInside) // vertices added!
			{
				firstIndexInside.push_back(fiInside);
				primitiveCountInside++;
				oldfiInside = fiInside;
			}
			if (oldfiOutside < fiOutside) // vertices added!
			{
				firstIndexOutside.push_back(fiOutside);
				primitiveCountOutside++;
				oldfiOutside = fiOutside;
			}
		}
	}

	SplitResult result;

	// inside
	result.inside = new PolyMesh(primitiveCountInside, positionsInside.size(), positionsInside.size() * 3);
	double* iPositions = new double[positionsInside.size()];
	//double* iUV = new double[positionsInside.size()];
	//double* iNormals = new double[positionsInside.size()];
	uint* iFia = new uint[positionsInside.size()];

	for (uint i = 0; i < positionsInside.size(); i++)
	{
		vec3 p = positionsInside[i];
		//vec3 uv = uvInside[i];
		//vec3 n = nInside[i];

		iPositions[i] = p.x;
		iPositions[i] = p.y;
		iPositions[i] = p.z;

		//iUV[i] = uv.x;
		//iUV[i] = uv.y;
		//iUV[i] = uv.z;

		//iNormals[i] = n.x;
		//iNormals[i] = n.y;
		//iNormals[i] = n.z;

		iFia[i] = i; // brute force indexing !!!
	}
	result.inside->setFirstIndexArray(iFia);
	result.inside->setVertexPositions(iPositions);
	//result.inside->setNormalIndexArray(iFia);
	//result.inside->setNormalPositions(iNormals);
	//result.inside->setUVIndexArray(iFia);
	//result.inside->setUVPositions(iUV);
	//result.inside->setNormalPositions(iNormals);

	result.inside->setDiffuseTexture(diffuseTextureHandle);
	result.inside->setSpecularTexture(specularTextureHandle);
	result.inside->setNormalTexture(normalTextureHandle);
	result.inside->setShader(shader);


	// outside
	result.outside = new PolyMesh(primitiveCountOutside, positionsOutside.size(), positionsOutside.size() * 3);
	double* oPositions = new double[positionsOutside.size()];
	//double* oUV = new double[positionsOutside.size()];
	//double* oNormals = new double[positionsOutside.size()];
	uint* oFia = new uint[positionsOutside.size()];

	for (uint i = 0; i < positionsOutside.size(); i++)
	{
		vec3 p = positionsOutside[i];
		//vec3 uv = uvOutside[i];
		//vec3 n = nOutside[i];

		oPositions[i] = p.x;
		oPositions[i] = p.y;
		oPositions[i] = p.z;

		//oUV[i] = uv.x;
		//oUV[i] = uv.y;
		//oUV[i] = uv.z;

		//oNormals[i] = n.x;
		//oNormals[i] = n.y;
		//oNormals[i] = n.z;

		oFia[i] = i; // brute force indexing !!!
	}
	result.outside->setFirstIndexArray(oFia);
	result.outside->setVertexPositions(oPositions);
	//result.outside->setNormalIndexArray(oFia);
	//result.outside->setNormalPositions(oNormals);
	//result.outside->setUVIndexArray(oFia);
	//result.outside->setUVPositions(oUV);
	//result.outside->setNormalPositions(oNormals);

	result.outside->setDiffuseTexture(diffuseTextureHandle);
	result.outside->setSpecularTexture(specularTextureHandle);
	result.outside->setNormalTexture(normalTextureHandle);
	result.outside->setShader(shader);

	return result;
}