/*
 * A simple error handler...
 */

#ifndef OPENCOLLADAERRORHANDLER_HPP_
#define OPENCOLLADAERRORHANDLER_HPP_

#include "COLLADASaxFWLIErrorHandler.h"

class OpenColladaErrorHandler :  public COLLADASaxFWL::IErrorHandler
{
public:
		OpenColladaErrorHandler();
		virtual ~OpenColladaErrorHandler();

		virtual bool handleError(const COLLADASaxFWL::IError* error);
};

#endif /* OPENCOLLADAERRORHANDLER_HPP_ */
