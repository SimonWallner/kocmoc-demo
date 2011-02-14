#include "Importer.hpp"

#include <common.hpp>
#include <renderer/ShaderManager.hpp>
#include <loader/ImageLoader.hpp>
#include <scene/PolyMesh.hpp>
#include <scene/PolyMeshNode.hpp>

#include <COLLADABUPlatform.h>
#include <COLLADAFWGeometry.h>
#include <COLLADAFWMesh.h>
#include <COLLADAFWVisualScene.h>
#include <COLLADAFWImage.h>
#include <COLLADABUURI.h>


using namespace kocmoc::loader::collada;

using kocmoc::renderer::ShaderManager;
using kocmoc::renderer::Shader;
using kocmoc::loader::ImageLoader;
using kocmoc::scene::PolyMesh;
using kocmoc::scene::PolyMeshNode;

using std::cout;
using std::endl;

Importer::~Importer() {}
Importer::Importer() {}

void Importer::cancel(const COLLADAFW::String& errorMessage)
{
	cout << "Import cancelled!" << endl;
}

void Importer::start()
{
	cout << "Import started" << endl;
}

void Importer::finish()
{
	cout << "Import finished" << endl;
}

bool Importer::writeGeometry (const COLLADAFW::Geometry* geometry)
{
	std::vector<unsigned int> firstIndices;
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;
	unsigned int firstIndex = 0;


	if (geometry->getType() == COLLADAFW::Geometry::GEO_TYPE_MESH)
	{
		const COLLADAFW::Mesh* mesh = static_cast<const COLLADAFW::Mesh* >(geometry);
		const COLLADAFW::MeshPrimitiveArray &meshPrimitives =  mesh->getMeshPrimitives();

		for (unsigned int i = 0; i < meshPrimitives.getCount(); i++)
		{
			const COLLADAFW::MeshPrimitive *meshPrimitive = meshPrimitives.getData()[i];
			
			if (meshPrimitive->getPrimitiveType() == COLLADAFW::MeshPrimitive::POLYGONS ||
				meshPrimitive->getPrimitiveType() == COLLADAFW::MeshPrimitive::TRIANGLES)
			{
				// copy/append indices
				const COLLADAFW::UIntValuesArray &colladaVertexIndices = meshPrimitive->getPositionIndices();
				const COLLADAFW::UIntValuesArray &colladaNormalIndices = meshPrimitive->getNormalIndices();
					const COLLADAFW::UIntValuesArray &colladaUVIndices = meshPrimitive->getUVCoordIndicesArray()[0]->getIndices();

				
				unsigned int indexCount = colladaVertexIndices.getCount();
				const unsigned int *vertexIndexData = colladaVertexIndices.getData();
				const unsigned int *normalIndexData = colladaNormalIndices.getData();
				const unsigned int *uvIndexData = colladaUVIndices.getData();

				for (unsigned int j = 0; j < indexCount; j++)
				{
					vertexIndices.push_back(vertexIndexData[j]);
					normalIndices.push_back(normalIndexData[j]);
					uvIndices.push_back(uvIndexData[j]);
				}


				unsigned int faceCount = meshPrimitive->getFaceCount();
				unsigned int grouped = meshPrimitive->getGroupedVertexElementsCount();

				for (unsigned int j = 0; j < faceCount; j++)
				{
					firstIndices.push_back(firstIndex);
					firstIndex += meshPrimitive->getGroupedVerticesVertexCount(j);
				}
			}
		}

		// return if neither polys of triangles where found
		if (firstIndices.size() == 0)
			return true;


		firstIndices.push_back(firstIndex); // IMPORTANT: add last first index

		unsigned int primitiveCount = firstIndices.size() - 1;
		unsigned int vertexIndexCount = vertexIndices.size();
		unsigned int vertexCount = mesh->getPositions().getValuesCount()/3; // stride = 3
	
		PolyMesh* poly = new PolyMesh(primitiveCount, vertexIndexCount, vertexCount);


	

		// UV...
		//double *uvArray = new double[mesh->getUVCoords().getFloatValues()->getCount()];
		//const float *uvData = mesh->getUVCoords().getFloatValues()->getData();

		//for (unsigned int i = 0; i < mesh->getUVCoords().getFloatValues()->getCount(); i++)
		//	uvArray[i] = static_cast<double >(uvData[i]);

		//poly->setUVIndexArray(uvArray);
		


		// first indices...
		unsigned int *fiaArray = new unsigned int[firstIndices.size()];
		for (unsigned int i = 0; i < firstIndices.size(); i++)
			fiaArray[i] = firstIndices[i];

		poly->setFirstIndexArray(fiaArray);


		// indices...
		unsigned int indexCount = vertexIndices.size();
		unsigned int *vertexIndexArray = new unsigned int[indexCount];
		unsigned int *normalIndexArray = new unsigned int[indexCount];
		unsigned int *uvIndexArray = new unsigned int[indexCount];

		for (unsigned int i = 0; i < indexCount; i++)
		{
			vertexIndexArray[i] = vertexIndices[i];
			normalIndexArray[i] = normalIndices[i];
			uvIndexArray[i] = uvIndices[i];
		}

		
		poly->setVertexIndexArray(vertexIndexArray);
		poly->setNormalIndexArray(normalIndexArray);
		poly->setUVIndexArray(uvIndexArray);


		// attributes
		// positions...
		unsigned int positionCount = mesh->getPositions().getFloatValues()->getCount();
		double *positionsArray = new double[positionCount];
		const float *positionsData =  mesh->getPositions().getFloatValues()->getData();
		
		for (unsigned int i = 0; i < positionCount; i++)
			positionsArray[i] = static_cast<double >(positionsData[i]);

		poly->setVertexPositions(positionsArray);

		// uv
		unsigned int uvCount = mesh->getUVCoords().getFloatValues()->getCount();
		double *uvPositions = new double[uvCount];
		const float *uvData = mesh->getUVCoords().getFloatValues()->getData();

		for (unsigned int i = 0; i < uvCount; i++)
			uvPositions[i] = static_cast<double >(uvData[i]);

		poly->setUVPositions(uvPositions);

		// normals
		unsigned int normalCount = mesh->getNormalsCount();
		double *normalPositions = new double[normalCount];
		const float *normalData = mesh->getNormals().getFloatValues()->getData();

		for (unsigned int i = 0; i < normalCount; i++)
			normalPositions[i] = static_cast<double >(normalData[i]);

		poly->setNormalPositions(normalPositions);



		
		// add shader and texture
		Shader *shader = ShaderManager::getInstance().load("base.vert", "base.frag");
		poly->setShader(shader);

		GLuint diffuseTex = ImageLoader::getInstance().loadImage(textures[0], true);
		GLuint specularTex = ImageLoader::getInstance().loadImage(textures[1], true);
		GLuint normalTex = ImageLoader::getInstance().loadImage(textures[2]);
		poly->setDiffuseTexture(diffuseTex);
		poly->setSpecularTexture(specularTex);
		poly->setNormalTexture(normalTex);

		
		// add to scene
		scene->add(poly);
	}

	return true;
}

void Importer::prepare()
{
	cout << "preparing..." << endl;
	scene = new PolyMeshNode;
}

PolyMeshNode* Importer::getSceneNode()
{
	return scene;
}


bool Importer::writeImage( const COLLADAFW::Image* image )
{

	std::string fileName = image->getImageURI().getPathFile();
	textures.push_back(fileName);
	return true;
}


bool Importer::writeVisualScene(const COLLADAFW::VisualScene* visualScene) {return true;}
bool Importer::writeGlobalAsset ( const COLLADAFW::FileInfo* asset ) {return true;}
bool Importer::writeScene ( const COLLADAFW::Scene* scene ) {return true;}
bool Importer::writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes ) {return true;}
bool Importer::writeMaterial( const COLLADAFW::Material* material ) {return true;}
bool Importer::writeEffect( const COLLADAFW::Effect* effect ) {return true;}
bool Importer::writeCamera( const COLLADAFW::Camera* camera ) {return true;}
bool Importer::writeLight( const COLLADAFW::Light* light ) {return true;}
bool Importer::writeAnimation( const COLLADAFW::Animation* animation ) {return true;}
bool Importer::writeAnimationList( const COLLADAFW::AnimationList* animationList ) {return true;}
//bool Importer::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData ) {return true;}
//bool Importer::writeController( const COLLADAFW::Controller* controller ) {return true;}
bool Importer::writeFormulas( const COLLADAFW::Formulas* formulas ) {return true;}
bool Importer::writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene ) {return true;}


