/*
 * A simple error handler...
 */

#ifndef KOCMOC_COLLADA_ERROR_HANDLER_HPP_
#define KOCMOC_COLLADA_ERROR_HANDLER_HPP_

#include <COLLADASaxFWLIErrorHandler.h>

namespace kocmoc
{

	class KocmocColladaErrorHandler :  public COLLADASaxFWL::IErrorHandler
	{
	public:
			KocmocColladaErrorHandler();
			virtual ~KocmocColladaErrorHandler();

			virtual bool handleError(const COLLADASaxFWL::IError* error);
	};
}

#endif /* KOCMOC_COLLADA_ERROR_HANDLER_HPP_ */
