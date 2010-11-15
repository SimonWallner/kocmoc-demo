#include "Exception.hpp"

Exception::Exception(string msg)
{
	message = msg;
}

Exception::~Exception(void)
{
}

string Exception::getMessage(void)
{
	return message;
}
