#include "KocmocColladaImporter.hpp"

#include "common.hpp"

#include <COLLADAFWGeometry.h>
#include <COLLADAFWMesh.h>
#include <COLLADAFWVisualScene.h>
#include "Shader.hpp"
#include "ImageLoader.hpp"
#include "ShaderManager.hpp"

using namespace kocmoc;

KocmocColladaImporter::KocmocColladaImporter()
{
	// TODO Auto-generated constructor stub

}

KocmocColladaImporter::~KocmocColladaImporter()
{
	// TODO Auto-generated destructor stub
}


void KocmocColladaImporter::cancel(const COLLADAFW::String& errorMessage)
{
	cout << "Import cancelled!" << endl;
}

void KocmocColladaImporter::start()
{
	cout << "Import started" << endl;
}

void KocmocColladaImporter::finish()
{
	cout << "Import finished" << endl;
}

bool KocmocColladaImporter::writeGeometry (const COLLADAFW::Geometry* geometry)
{
	cout << "receiving geometry... " << endl;

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> firstIndices;
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
				const COLLADAFW::UIntValuesArray &positionIndices = meshPrimitive->getPositionIndices();
				const unsigned int *data = positionIndices.getData();
				unsigned int indexCount = positionIndices.getCount();

				for (unsigned int j = 0; j < indexCount; j++)
				{
					vertexIndices.push_back(data[j]);
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


		// positions...
		double *positionsArray = new double[mesh->getPositions().getFloatValues()->getCount()];
		const float *positionsData =  mesh->getPositions().getFloatValues()->getData();
		
		for (unsigned int i = 0; i < mesh->getPositions().getFloatValues()->getCount(); i++)
			positionsArray[i] = static_cast<double>(positionsData[i]);

		poly->setVertexPositions(positionsArray);


		// indices...
		unsigned int *indicesArray = new unsigned int[vertexIndices.size()];
		for (unsigned int i = 0; i < vertexIndices.size(); i++)
			indicesArray[i] = vertexIndices[i];
		
		poly->setVertexIndexArray(indicesArray);


		// first indices...
		unsigned int *fiaArray = new unsigned int[firstIndices.size()];
		for (unsigned int i = 0; i < firstIndices.size(); i++)
			fiaArray[i] = firstIndices[i];

		poly->setFirstIndexArray(fiaArray);


		
		
		// add shader and texture
		Shader *shader = ShaderManager::getInstance().load("base.vert", "base.frag");
		poly->setShader(shader);

		GLuint tex = ImageLoader::getInstance().loadImage("uv_test.png");
		poly->setTexture(tex);

		// add to scene
		scene->add(poly);
	}

	return true;
}

void KocmocColladaImporter::prepare()
{
	cout << "preparing..." << endl;
	scene = new KocmocScene;
}

KocmocScene* KocmocColladaImporter::getScene()
{
	return scene;
}




bool KocmocColladaImporter::writeVisualScene(const COLLADAFW::VisualScene* visualScene) {return true;}
bool KocmocColladaImporter::writeGlobalAsset ( const COLLADAFW::FileInfo* asset ) {return true;}
bool KocmocColladaImporter::writeScene ( const COLLADAFW::Scene* scene ) {return true;}
bool KocmocColladaImporter::writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes ) {return true;}
bool KocmocColladaImporter::writeMaterial( const COLLADAFW::Material* material ) {return true;}
bool KocmocColladaImporter::writeEffect( const COLLADAFW::Effect* effect ) {return true;}
bool KocmocColladaImporter::writeCamera( const COLLADAFW::Camera* camera ) {return true;}
bool KocmocColladaImporter::writeImage( const COLLADAFW::Image* image ) {return true;}
bool KocmocColladaImporter::writeLight( const COLLADAFW::Light* light ) {return true;}
bool KocmocColladaImporter::writeAnimation( const COLLADAFW::Animation* animation ) {return true;}
bool KocmocColladaImporter::writeAnimationList( const COLLADAFW::AnimationList* animationList ) {return true;}
//bool KocmocColladaImporter::writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData ) {return true;}
//bool KocmocColladaImporter::writeController( const COLLADAFW::Controller* controller ) {return true;}
bool KocmocColladaImporter::writeFormulas( const COLLADAFW::Formulas* formulas ) {return true;}
bool KocmocColladaImporter::writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene ) {return true;}


