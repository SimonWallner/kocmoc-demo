#include "RenderMesh.hpp"

#include "Shader.hpp"

#include <scene/PolyMesh.hpp>
#include <scene/LineGizmo.hpp>
#include <camera/Camera.hpp>
#include <util/util.hpp>
#include <util/Property.hpp>
#include <loader/ImageLoader.hpp>
#include <Kocmoc.hpp>


#include <vector>

using namespace kocmoc::renderer;
using kocmoc::scene::PolyMesh;
using kocmoc::scene::LineGizmo;
using kocmoc::camera::Camera;
using kocmoc::loader::ImageLoader;
using kocmoc::util::Property;

using glm::mat4;
using glm::mat3;
using glm::dvec3;

using std::vector;


RenderMesh::RenderMesh(PolyMesh* _mesh, Shader* _shader)
	: mesh(_mesh)
	, shader(_shader)
	, modelMatrix(mat4(1))
	, isUploaded(false)
	, uniformsAreSet(false)
	, debugDrawMeshGizmo(Property("debugDrawMeshGizmo"))
{
	originGizmo = util::generator::generateOriginGizmo();
	boundingBox = util::generator::generateUnitCube();

	PolyMesh::BoundingBox bb = mesh->calculateBoundingBox();
	dvec3 center = (bb.max + bb.min) / 2.0;
	dvec3 scale = bb.max - bb.min;

	bbTransform = glm::scale(glm::translate(center), scale);
};

RenderMesh::~RenderMesh()
{
	delete originGizmo;
	delete boundingBox;
	delete mesh;
}

void RenderMesh::setModelMatrix(mat4 _modelMatrix)
{
	modelMatrix = _modelMatrix;
	
}

void RenderMesh::draw(mat4 parentTransform, Camera *camera)
{
	if (shader->getIsUploaded() == false)
		shader->upload();

	if (!uniformsAreSet || true)
	{
		uniformProjectionMatrix = shader->get_uniform_location("projectionMatrix");
		uniformViewMatrix = shader->get_uniform_location("viewMatrix");
		uniformModelMatrix = shader->get_uniform_location("modelMatrix");
		uniformNormalMatrix = shader->get_uniform_location("normalMatrix");
		uniformSunDirection = shader->get_uniform_location("sunDirection");
		uniformCameraPosition = shader->get_uniform_location("cameraPosition");
		uniformReileighCoefficient = shader->get_uniform_location("reileighCoefficient");
		uniformMieCoefficient = shader->get_uniform_location("mieCoefficient");
		uniformMieDirectionalG = shader->get_uniform_location("mieDirectionalG");
		uniformTurbidity = shader->get_uniform_location("turbidity");
		uniformsAreSet = true;
	}

	if (isUploaded == false)
		uploadData();

	mat4 leafTransform = parentTransform * modelMatrix;
	//mat3 normalMatrix = inverseTranspose(mat3(modelMatrix));
	mat3 normalMatrix = glm::inverse(glm::transpose(mat3(modelMatrix)));


	shader->bind();

	
	if (uniformCameraPosition >= 0)
		glUniform3fv(uniformCameraPosition, 1, glm::value_ptr(camera->getPosition()));

	if (uniformProjectionMatrix >= 0)
		glUniformMatrix4fv(uniformProjectionMatrix, 1, GL_FALSE, glm::value_ptr(camera->getProjectionMatrix()));

	if (uniformViewMatrix >= 0)
		glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));

	if (uniformModelMatrix >= 0)
		glUniformMatrix4fv(uniformModelMatrix, 1, GL_FALSE, glm::value_ptr(leafTransform));

	if (uniformNormalMatrix >= 0)
		glUniformMatrix3fv(uniformNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));	

	if (uniformSunDirection >= 0)
		glUniform3fv(uniformSunDirection, 1, glm::value_ptr(Kocmoc::getInstance().sunDirection));	
	
	if (uniformReileighCoefficient >= 0)
		glUniform1f(uniformReileighCoefficient, Kocmoc::getInstance().reileighCoefficient);	

	if (uniformMieCoefficient >= 0)
		glUniform1f(uniformMieCoefficient, Kocmoc::getInstance().mieCoefficient);	

	if (uniformMieDirectionalG >= 0)
		glUniform1f(uniformMieDirectionalG, Kocmoc::getInstance().mieDirectionalG);
	
	if (uniformTurbidity >= 0)
		glUniform1f(uniformTurbidity, Kocmoc::getInstance().turbidity);


	for (RenderTextureList::const_iterator ci = renderTextures.begin();
		ci != renderTextures.end();
		ci++)
	{
		glActiveTexture(GL_TEXTURE0 + ci->textureUnit);
		glBindTexture(GL_TEXTURE_2D, ci->handle);
	}


	//{ // FIXME: remove this! used for easy shader reloading.
	//	GLint location;
	//	if ((location = shader->get_uniform_location(DIFFUSE_SAMPLER_NAME)) > 0)
	//		glUniform1i(location, 0);
	//	if ((location = shader->get_uniform_location(SPECULAR_SAMPLER_NAME)) > 0)
	//		glUniform1i(location, 1);
	//	if ((location = shader->get_uniform_location(NORMAL_SAMPLER_NAME)) > 0)
	//		glUniform1i(location, 2);
	//	if ((location = shader->get_uniform_location(SHADOW_SAMPLER_NAME)) > 0)
	//		glUniform1i(location, 3);
	//}


	glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, triangulatedVertexIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	shader->unbind();

	// extras, debug etc...
	if (kocmoc::Kocmoc::paramMapBool[symbolize("debugDrawMeshGizmo")])
	{
		originGizmo->draw(leafTransform, camera);
		boundingBox->draw(leafTransform * bbTransform, camera);
	}
}

void RenderMesh::uploadData()
{
	// TODO: streamline the setup process in one location
	setUpTextures();

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);


	// reindex vertex data and convert to float
	// brute force expansion, (for now...)
	Shader::VertexAttributeSemanticList semantics = shader->getSemantics();

	for (Shader::VertexAttributeSemanticList::const_iterator ci = semantics.begin();
		ci != semantics.end();
		ci++)
	{
		// TODO: make find error proof!
		const PolyMesh::VertexAttribute& attribute = mesh->vertexAttributes.find(ci->attributeName)->second;

		uint indexCount = mesh->vertexIndexCount;
		uint dataLength = indexCount * attribute.stride;

		float* reindexedData = new float[dataLength];

		for (uint i = 0; i < indexCount; i++)
		{
			for (uint j = 0; j < attribute.stride; j++)
			{
				reindexedData[i*attribute.stride + j] = (float)attribute.attributeData[attribute.indices[i] * attribute.stride + j];
			}
		}

		GLuint handle = 0;
		glGenBuffers(1, &handle);
		glBindBuffer(GL_ARRAY_BUFFER, handle);
		glBufferData(GL_ARRAY_BUFFER, dataLength * sizeof(float), reindexedData, GL_STATIC_DRAW);
		glVertexAttribPointer(ci->attributeLocation, attribute.stride, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(ci->attributeLocation);

		delete[] reindexedData;
	}

	
	
	// triangulate indices
	// upload

	uint* reindexedIndices = new uint[mesh->vertexIndexCount];
	for (uint i = 0; i < mesh->vertexIndexCount; i++)
		reindexedIndices[i] = i;

	vector<unsigned int> triangulatedIndices;

	for (uint i = 0; i < mesh->primitiveCount; i++)
	{
		uint count = mesh->firstIndexArray[i+1] - mesh->firstIndexArray[i];

		if (count == 3)
		{
			// just upload
			triangulatedIndices.push_back(reindexedIndices[mesh->firstIndexArray[i]]);
			triangulatedIndices.push_back(reindexedIndices[mesh->firstIndexArray[i]+1]);
			triangulatedIndices.push_back(reindexedIndices[mesh->firstIndexArray[i]+2]);
		}
		else if (count > 3)
		{
			// triangulate
			unsigned int firstIndex = mesh->firstIndexArray[i];
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
	GLuint handle;
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangulatedVertexIndexCount * sizeof(unsigned int), &triangulatedIndices[0], GL_STATIC_DRAW);

	delete[] reindexedIndices;

	isUploaded = true;
}

void RenderMesh::setUpTextures()
{
	GLuint textureUnit = 0;
	shader->bind();

	Shader::TextureSemanticList semantics = shader->getTextureSemantics();
	
	for (Shader::TextureSemanticList::const_iterator ci = semantics.begin();
		ci != semantics.end();
		ci++)
	{
		const PolyMesh::TextureMap::const_iterator ti = mesh->textures.find(ci->textureName);
		if (ti != mesh->textures.end())
		{
			const PolyMesh::Texture& texture = ti->second;
			GLuint handle = ImageLoader::getInstance().loadImage(texture.fileName);

			RenderTexture renderTexture(handle, textureUnit);
			renderTextures.push_back(renderTexture);

			GLint location = shader->get_uniform_location(ci->samplerName);
			if (location >= 0)
				glUniform1i(location, textureUnit);

			textureUnit++;
		}
	}

	shader->unbind();
}

uint RenderMesh::getVertexCount()
{
	return mesh->vertexIndexCount;
}
