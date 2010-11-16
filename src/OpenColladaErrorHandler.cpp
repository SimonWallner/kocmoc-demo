#include "OpenColladaErrorHandler.hpp"
#include "common.hpp"

OpenColladaErrorHandler::OpenColladaErrorHandler()
{
	// TODO Auto-generated constructor stub
}

OpenColladaErrorHandler::~OpenColladaErrorHandler()
{
	// TODO Auto-generated destructor stub
}


bool OpenColladaErrorHandler::handleError(const COLLADASaxFWL::IError* error)
{
	cout << "An error occured: " << error->getFullErrorMessage();
}
