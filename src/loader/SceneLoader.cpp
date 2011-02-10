/*
 * SceneLoader.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "SceneLoader.hpp"

#include <util/Property.hpp>
#include <util/util.hpp>

#include <COLLADAFW.h>

using namespace kocmoc;


SceneLoader *SceneLoader::instance = NULL;

SceneLoader &SceneLoader::getInstance()
{
	if (!instance)
		instance = new SceneLoader();

	return *instance;
}

void SceneLoader::Destroy()
{
	delete instance;
	instance = NULL;
}


SceneLoader::SceneLoader() :
		importer(),
		saxLoader(&errorHandler),
		colladaRoot(&saxLoader, &importer),
		pathPrefix(util::Property("ModelsRootFolder")),
		alternativePathPrefix(util::Property("alternativeModelsRootFolder"))
{}

SceneLoader::~SceneLoader()
{
	// TODO Auto-generated destructor stub
}

Scene* SceneLoader::load(string name)
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
