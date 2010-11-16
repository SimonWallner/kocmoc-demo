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


bool KocmocColladaImporter::writeGlobalAsset ( const COLLADAFW::FileInfo* asset ) {return true;}
bool KocmocColladaImporter::writeScene ( const COLLADAFW::Scene* scene ) {return true;}
bool KocmocColladaImporter::writeVisualScene ( const COLLADAFW::VisualScene* visualScene ) {return true;}
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


