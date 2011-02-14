/*
 * Importer to read collada files using openCollada
 *
 * Code based on the xray collada viewer by H.Fink
 */

#ifndef KOCMOC_LOADER_COLLADA_IMPORTER_HPP_
#define KOCMOC_LOADER_COLLADA_IMPORTER_HPP_

#include <COLLADAFWIWriter.h>
#include <COLLADAFWPrerequisites.h>

#include <vector>

using COLLADAFW::String;
using COLLADAFW::Geometry;

namespace kocmoc
{
	namespace scene
	{
		class PolyMeshNode;
	}

	namespace loader
	{
		namespace collada
		{

			typedef std::vector<std::string > TextureSet;

			class Importer : public COLLADAFW::IWriter
			{
			public:
				Importer();
				virtual ~Importer();

				/** prepare the scene etc... */
				void prepare(void);

				/**
				 * Get the resulting scene
				 */
				scene::PolyMeshNode* getSceneNode(void);


				// overwrite functions from IWriter

				/** This is the method called. The writer hast to prepare to receive data.*/
				void start();

				/** This method is called after the last write* method. No other methods will be called after this.*/
				void finish();

				/** This method will be called if an error in the loading process occurred and the loader cannot
				continue to to load. The writer should undo all operations that have been performed.
				@param errorMessage A message containing informations about the error that occurred.
				*/
				void cancel(const String& errorMessage);

				/** When this method is called, the writer must write the geometry.
				@return The writer should return true, if writing succeeded, false otherwise.*/
				bool writeGeometry(const Geometry* geometry);

			

				// unimplemented methods
				virtual bool writeVisualScene ( const COLLADAFW::VisualScene* visualScene );
				virtual bool writeGlobalAsset ( const COLLADAFW::FileInfo* asset );
				virtual bool writeScene ( const COLLADAFW::Scene* scene );
				virtual bool writeLibraryNodes ( const COLLADAFW::LibraryNodes* libraryNodes );
				virtual bool writeMaterial( const COLLADAFW::Material* material );
				virtual bool writeEffect( const COLLADAFW::Effect* effect );
				virtual bool writeCamera( const COLLADAFW::Camera* camera );
				virtual bool writeImage( const COLLADAFW::Image* image );
				virtual bool writeLight( const COLLADAFW::Light* light );
				virtual bool writeAnimation( const COLLADAFW::Animation* animation );
				virtual bool writeAnimationList( const COLLADAFW::AnimationList* animationList );
				virtual bool writeSkinControllerData( const COLLADAFW::SkinControllerData* skinControllerData ) { return true; }
				virtual bool writeController( const COLLADAFW::Controller* Controller ) { return true; }
				virtual bool writeFormulas( const COLLADAFW::Formulas* formulas );
				virtual bool writeKinematicsScene( const COLLADAFW::KinematicsScene* kinematicsScene );

			private:
				scene::PolyMeshNode* scene;

				/** Ordered set of textures
				 * 1. diffuse
				 * 2. specular
				 * 3. normal
				 */
				TextureSet textures;
			};
		}
	}
}
#endif