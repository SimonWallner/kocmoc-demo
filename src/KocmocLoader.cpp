/*
 * KocmocLoader.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "KocmocLoader.hpp"
#include "common.hpp"

#include <COLLADAFW.h>

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


KocmocLoader::KocmocLoader()
{
	// TODO Auto-generated constructor stub
}

KocmocLoader::~KocmocLoader()
{
	// TODO Auto-generated destructor stub
}

PolyMesh KocmocLoader::load(string name)
{
	colladaRoot = COLLADAFW::Root(saxLoader, importer);
}
