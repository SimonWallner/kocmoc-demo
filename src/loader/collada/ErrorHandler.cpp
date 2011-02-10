#include "ErrorHandler.hpp"
#include "common.hpp"

using namespace kocmoc;

ErrorHandler::ErrorHandler()
{
	// TODO Auto-generated constructor stub
}

ErrorHandler::~ErrorHandler()
{
	// TODO Auto-generated destructor stub
}


bool ErrorHandler::handleError(const COLLADASaxFWL::IError* error)
{
	cout << "some error occured! " << endl;
	return true;
}
