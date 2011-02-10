/*
 * A simple error handler...
 */

#ifndef _ERROR_HANDLER_HPP_
#define _ERROR_HANDLER_HPP_

#include <COLLADASaxFWLIErrorHandler.h>

namespace kocmoc
{

	class ErrorHandler :  public COLLADASaxFWL::IErrorHandler
	{
	public:
			ErrorHandler();
			virtual ~ErrorHandler();

			virtual bool handleError(const COLLADASaxFWL::IError* error);
	};
}

#endif /* _ERROR_HANDLER_HPP_ */
