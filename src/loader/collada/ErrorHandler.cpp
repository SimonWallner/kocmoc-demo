#include "ErrorHandler.hpp"

#include <common.hpp>

using namespace kocmoc::loader::collada;

using std::cout;
using std::endl;

bool ErrorHandler::handleError(const COLLADASaxFWL::IError* error)
{
	cout << "some error occured! " << endl;
	return true;
}


ErrorHandler::ErrorHandler() {}
ErrorHandler::~ErrorHandler() {}