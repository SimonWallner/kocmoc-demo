/*
 * Loader.cpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#include "Loader.hpp"
#include "common.hpp"

#include <COLLADAFW.h>

Loader *Loader::instance = NULL;

Loader &Loader::getInstance()
{
	if (!instance)
		instance = new Loader();

	return *instance;
}

void Loader::Destroy()
{
	delete instance;
	instance = NULL;
}


Loader::Loader()
{
	// TODO Auto-generated constructor stub

}

Loader::~Loader()
{
	// TODO Auto-generated destructor stub
}

PolyMesh Loader::load(string name)
{
	colladaRoot = COLLADAFW::Root(loader, importer);
}
