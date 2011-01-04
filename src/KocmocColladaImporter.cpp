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

	if (geometry->getType() == COLLADAFW::Geometry::GEO_TYPE_MESH)
	{
		//cast to mesh
		const COLLADAFW::Mesh* mesh = static_cast<const COLLADAFW::Mesh* >(geometry);

		unsigned int vertexCount =  mesh->getPositions().getValuesCount()/3;
		unsigned int primitiveCount = const_cast<COLLADAFW::Mesh* >(mesh)->getTrianglesTriangleCount();
		unsigned int vertexIndexCount = primitiveCount * 3;
		
		unsigned int *indices = new unsigned int[vertexIndexCount];

		const COLLADAFW::MeshPrimitiveArray &primitives =  mesh->getMeshPrimitives();
		
		
		for (unsigned int i = 0; i < primitives.getCount(); i++)
		{
			const COLLADAFW::MeshPrimitive *primitive = primitives.getData()[i];
			if (primitive->getPrimitiveType() == COLLADAFW::MeshPrimitive::TRIANGLES)
			{
				// only use triangles, for now
				const COLLADAFW::UIntValuesArray &positionIndices = primitive->getPositionIndices();
				const unsigned int *data = positionIndices.getData();

				for (unsigned int j = 0; j < vertexIndexCount; j++)
				{
					indices[j] = data[j];
				}
			}
		}

		PolyMesh* poly = new PolyMesh(primitiveCount, vertexIndexCount, vertexCount);
		const COLLADAFW::FloatArray* arr =  mesh->getPositions().getFloatValues();
		const COLLADAFW::FloatArray* arrNormals = mesh->getNormals().getFloatValues();

		int count = mesh->getPositions().getValuesCount();


		double *positions = new double[arr->getCount()];
		const float *data = arr->getData();
		double *normals = new double[arrNormals->getCount()];
		const float *normalsData = arrNormals->getData();

		for (unsigned int i = 0; i < arr->getCount(); i++)
		{
			positions[i] = static_cast<double>(data[i]);
			normals[i] = static_cast<double>(normalsData[i]);
		}
		poly->setVertexPositions(positions);
		poly->setVertexIndexArray(indices);
		//poly->setVertexNormals(normals);
		//poly->setUV0(positions);

		// add shader to poly
		Shader *shader = ShaderManager::getInstance().load("base.vert", "base.frag");
		poly->setShader(shader);

		// add texture
		GLuint tex = ImageLoader::getInstance().loadImage("uv_test.png");
		poly->setTexture(tex);

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


