#include "KocmocColladaErrorHandler.hpp"
#include "common.hpp"

KocmocColladaErrorHandler::KocmocColladaErrorHandler()
{
	// TODO Auto-generated constructor stub
}

KocmocColladaErrorHandler::~KocmocColladaErrorHandler()
{
	// TODO Auto-generated destructor stub
}


bool KocmocColladaErrorHandler::handleError(const COLLADASaxFWL::IError* error)
{
	cout << "some error occured! " << endl;
}
