#include "Property.hpp"

#include "PropertiesFileParser.hpp"

using namespace kocmoc::util;

Property::Property(std::string _name) : name(_name) {}

Property::~Property(void) {}

Property::operator bool ()
{
	bool foo;
	PropertiesFileParser::GetInstance().getProperty(name, &foo);
	return foo;
}

Property::operator int()
{
	int foo;
	PropertiesFileParser::GetInstance().getProperty(name, &foo);
	return foo;
}

Property::operator float()
{
	float foo;
	PropertiesFileParser::GetInstance().getProperty(name, &foo);
	return foo;
}


Property::operator std::string()
{
	std::string foo;
	PropertiesFileParser::GetInstance().getProperty(name, &foo);
	return foo;
}

