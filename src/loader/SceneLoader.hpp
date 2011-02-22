/*
 * SceneLoader.hpp
 *
 *  Created on: 15 Nov 2010
 *      Author: simon
 */

#ifndef KOCMOC_SCENE_SCENE_LOADER_HPP_
#define KOCMOC_SCENE_SCENE_LOADER_HPP_

#include "collada/Importer.hpp"
#include "collada/ErrorHandler.hpp"

#include <common.hpp>

#include <COLLADASaxFWLLoader.h>
#include <COLLADAFWRoot.h>

#include <string>

namespace kocmoc
{
	namespace scene
	{
		class RenderMeshNode;
	}

	namespace loader
	{

		class SceneLoader
		{
		public:

			/**
			 * Get the singleton instance
			 */
			static SceneLoader& getInstance(void);

			/**
			 * Destroy the singleton instance
			 */
			static void Destroy(void);

			/**
			 * Load the model with the given name into a new polyMesh.
			 * @param name The name of the model
			 * @return the polyMesh representing that model
			 */
			scene::RenderMeshNode* load(std::string name);

		private:

			static SceneLoader* instance;

			collada::Importer importer;
			COLLADAFW::Root colladaRoot;
			COLLADASaxFWL::Loader saxLoader;
			collada::ErrorHandler errorHandler;

			std::string pathPrefix;
			/** This is the path for resources that are not in the repo */
			std::string alternativePathPrefix;

			SceneLoader(void);
			SceneLoader(const SceneLoader &cc);
			~SceneLoader();
		};
	}
}

#endif
