#include "OpenColladaImporter.hpp"

#include "common.hpp"

OpenColladaImporter::OpenColladaImporter()
{
	// TODO Auto-generated constructor stub

}

OpenColladaImporter::~OpenColladaImporter()
{
	// TODO Auto-generated destructor stub
}


void OpenColladaImporter::cancel(const COLLADAFW::String& errorMessage)
{
	cout << "Import cancelled!" << endl;
}

void OpenColladaImporter::start()
{
	cout << "Import started" << endl;
}

void OpenColladaImporter::finish()
{
	cout << "Import finished" << endl;
}

bool writeGeometry (const COLLADAFW::Geometry* geometry )
{
	cout << "receiving geometry: " << endl;
}
