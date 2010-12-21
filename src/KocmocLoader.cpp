/*
 * KocmocLoader.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "KocmocLoader.hpp"
#include "PropertiesFileParser.hpp"

#include <COLLADAFW.h>

using namespace kocmoc;

KocmocLoader *KocmocLoader::instance = NULL;

KocmocLoader &KocmocLoader::getInstance()
{
	if (!instance)
		instance = new KocmocLoader();

	return *instance;
}

void KocmocLoader::Destroy()
{
	delete instance;
	instance = NULL;
}


KocmocLoader::KocmocLoader() :
		importer(),
		saxLoader(&errorHandler),
		colladaRoot(&saxLoader, &importer)
{
	util::PropertiesFileParser::GetInstance().getProperty("ModelsRootFolder", &pathPrefix);
}

KocmocLoader::~KocmocLoader()
{
	// TODO Auto-generated destructor stub
}

KocmocScene* KocmocLoader::load(string name)
{
	importer.prepare();

	string path = pathPrefix + name;
	bool success = colladaRoot.loadDocument(path);

	return importer.getScene();
}
