#include "Exception.hpp"

using namespace kocmoc;

Exception::Exception(std::string msg)
{
	message = msg;
}

Exception::~Exception(void)
{
}

std::string Exception::getMessage(void)
{
	return message;
}
