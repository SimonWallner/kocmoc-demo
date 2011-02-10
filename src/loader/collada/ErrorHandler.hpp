/*
 * A simple error handler...
 */

#ifndef KOCMOC_LOADER_COLLADA_ERROR_HANDLER_HPP_
#define KOCMOC_LOADER_COLLADA_ERROR_HANDLER_HPP_

#include <COLLADASaxFWLIErrorHandler.h>

namespace kocmoc
{
	namespace loader
	{
		namespace collada
		{

			class ErrorHandler :  public COLLADASaxFWL::IErrorHandler
			{
			public:
					ErrorHandler();
					virtual ~ErrorHandler();

					virtual bool handleError(const COLLADASaxFWL::IError* error);
			};
		}
	}
}
#endif