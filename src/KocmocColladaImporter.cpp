#include "KocmocColladaImporter.hpp"

#include "common.hpp"

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
	cout << "receiving geometry: " << endl;
}


bool KocmocColladaImporter::writeGlobalAsset ( const FileInfo* asset ) {return true;}
bool KocmocColladaImporter::writeScene ( const Scene* scene ) {return true;}
bool KocmocColladaImporter::writeVisualScene ( const VisualScene* visualScene ) {return true;}
bool KocmocColladaImporter::writeLibraryNodes ( const LibraryNodes* libraryNodes ) {return true;}
bool KocmocColladaImporter::writeGeometry ( const Geometry* geometry ) {return true;}
bool KocmocColladaImporter::writeMaterial( const Material* material ) {return true;}
bool KocmocColladaImporter::writeEffect( const Effect* effect ) {return true;}
bool KocmocColladaImporter::writeCamera( const Camera* camera ) {return true;}
bool KocmocColladaImporter::writeImage( const Image* image ) {return true;}
bool KocmocColladaImporter::writeLight( const Light* light ) {return true;}
bool KocmocColladaImporter::writeAnimation( const Animation* animation ) {return true;}
bool KocmocColladaImporter::writeAnimationList( const AnimationList* animationList ) {return true;}
bool KocmocColladaImporter::writeSkinControllerData( const SkinControllerData* skinControllerData ) {return true;}
bool KocmocColladaImporter::writeController( const Controller* controller ) {return true;}
bool KocmocColladaImporter::writeFormulas( const Formulas* formulas ) {return true;}
bool KocmocColladaImporter::writeKinematicsScene( const KinematicsScene* kinematicsScene ) {return true;}


