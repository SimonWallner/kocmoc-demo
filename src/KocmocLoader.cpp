/*
 * KocmocLoader.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "KocmocLoader.hpp"
#include "Property.hpp"
#include "utility.hpp"

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
		colladaRoot(&saxLoader, &importer),
		pathPrefix(util::Property("ModelsRootFolder")),
		alternativePathPrefix(util::Property("alternativeModelsRootFolder"))
{}

KocmocLoader::~KocmocLoader()
{
	// TODO Auto-generated destructor stub
}

KocmocScene* KocmocLoader::load(string name)
{
	std::cout << "trying to load: " << name << std::endl;

	importer.prepare();
	bool success = false;

	string path = pathPrefix + name;
	
	if (util::file_exists(path))
		success = colladaRoot.loadDocument(path);
	else
	{
		std::cout << "file not found, trying alternative location... " << std::endl;
		path = alternativePathPrefix + name;
		success = colladaRoot.loadDocument(path);
	}

	return importer.getScene();
}
