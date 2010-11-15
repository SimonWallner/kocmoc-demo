/*
 * Importer to read collada files using openCollada
 */

#ifndef OPENCOLLADAIMPORTER_HPP_
#define OPENCOLLADAIMPORTER_HPP_

#include <COLLADAFWIWriter.h>
#include <COLLADAFWPrerequisites.h>

using COLLADAFW::String;
using COLLADAFW::Geometry;

class OpenColladaImporter : public COLLADAFW::IWriter
{
public:
	OpenColladaImporter();
	virtual ~OpenColladaImporter();



	// overwrite functions from IWriter

	/** This method will be called if an error in the loading process occurred and the loader cannot
	continue to to load. The writer should undo all operations that have been performed.
	@param errorMessage A message containing informations about the error that occurred.
	*/
	virtual void cancel(const String& errorMessage);

	/** This is the method called. The writer hast to prepare to receive data.*/
	virtual void start();

	/** This method is called after the last write* method. No other methods will be called after this.*/
	virtual void finish();

	/** When this method is called, the writer must write the geometry.
	@return The writer should return true, if writing succeeded, false otherwise.*/
	virtual bool writeGeometry (const COLLADAFW::Geometry* geometry);
};

#endif /* OPENCOLLADAIMPORTER_HPP_ */
