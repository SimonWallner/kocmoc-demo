/*
 * SceneLoader.hpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#ifndef KOCMOC_LOADER_HPP_
#define KOCMOC_LOADER_HPP_

#include "collada/Importer.hpp"
#include "collada/ErrorHandler.hpp"

#include <scene/PolyMesh.hpp>
#include <scene/Scene.hpp>
#include <common.hpp>

#include <COLLADASaxFWLLoader.h>
#include <COLLADAFWRoot.h>

#include <string>

namespace kocmoc
{

	class SceneLoader
	{
	public:

		/**
		 * Get the singleton instance
		 */
		static SceneLoader &getInstance(void);

		/**
		 * Destroy the singleton instance
		 */
		static void Destroy(void);

		/**
		 * Load the model with the given name into a new polyMesh.
		 * @param name The name of the model
		 * @return the polyMesh representing that model
		 */
		Scene* load(std::string name);

	private:

		static SceneLoader *instance;

		Importer importer;
		COLLADAFW::Root colladaRoot;
		COLLADASaxFWL::Loader saxLoader;
		ErrorHandler errorHandler;

		string pathPrefix;
		/** This is the path for resources that are not in the repo */
		std::string alternativePathPrefix;

		SceneLoader(void);
		SceneLoader(const SceneLoader &cc);
		~SceneLoader();
	};
}

#endif /* KOCMOC_LOADER_HPP_ */
