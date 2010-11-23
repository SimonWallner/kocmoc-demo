/*
 * KocmocLoader.hpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#ifndef KOCMOC_LOADER_HPP_
#define KOCMOC_LOADER_HPP_

#include <string>
#include "PolyMesh.hpp"
#include "KocmocScene.hpp"
#include "KocmocColladaImporter.hpp"
#include "KocmocColladaErrorHandler.hpp"
#include "common.hpp"

#include <COLLADASaxFWLLoader.h>
#include <COLLADAFWRoot.h>


class KocmocLoader
{
public:

	/**
	 * Get the singleton instance
	 */
	static KocmocLoader &getInstance(void);

	/**
	 * Destroy the singleton instance
	 */
	static void Destroy(void);

	/**
	 * Load the model with the given name into a new polyMesh.
	 * @param name The name of the model
	 * @return the polyMesh representing that model
	 */
	KocmocScene load(std::string name);

private:

	static KocmocLoader *instance;

	KocmocColladaImporter importer;
	COLLADAFW::Root colladaRoot;
	COLLADASaxFWL::Loader saxLoader;
	KocmocColladaErrorHandler errorHandler;

	string pathPrefix;

	KocmocLoader(void);
	KocmocLoader(const KocmocLoader &cc);
	~KocmocLoader();
};

#endif /* KOCMOC_LOADER_HPP_ */
