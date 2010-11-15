/*
 * Loader.hpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#ifndef LOADER_HPP_
#define LOADER_HPP_

#include <string>
#include "PolyMesh.hpp"
#include "OpenColladaImporter.hpp"

#include <COLLADASaxFWL.h>


class Loader
{
public:

	/**
	 * Get the singleton instance
	 */
	static Loader &getInstance(void);

	/**
	 * Destroy the singleton instance
	 */
	static void Destroy(void);

	/**
	 * Load the model with the given name into a new polyMesh.
	 * @param name The name of the model
	 * @return the polyMesh representing that model
	 */
	PolyMesh load(std::string name);

private:

	static Loader *instance;

	OpenColladaImporter importer;
	COLLADAFW::Root colladaRoot;
	COLLADASaxFWL::Loader saxLoader;

	Loader(void);
	Loader(const Loader &cc);
	~Loader();
};

#endif /* LOADER_HPP_ */
