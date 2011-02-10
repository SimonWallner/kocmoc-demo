#include "PropertiesFileParser.hpp"

#include <fstream>

using namespace kocmoc::util;

PropertiesFileParser::PropertiesFileParser(void)
{
	//default values
	//cout << "pre-populating props cache with default values" << endl;
	cache["width"] = "1280";
	cache["height"] = "720";
	cache["fullscreen"] = "0";
}

PropertiesFileParser::~PropertiesFileParser(void)
{
}

PropertiesFileParser& PropertiesFileParser::GetInstance()
{
	static PropertiesFileParser instance;
	return instance;
}

bool PropertiesFileParser::isspacesonly(const std::string& line)
{
	for (unsigned int i = 0; i < line.length(); ++i)
	{
		if (!isspace(line[i]))
		{
			return false;
		}
	}
	return true;
}

void PropertiesFileParser::getnextline(std::istream& is, std::string& line) 
{
	while (!is.eof()) {
		getline(is, line);

		if (!isspacesonly(line)) return;
	}
}

bool PropertiesFileParser::parse()
{
	//cout << "parsing properties file: " << CONFIG_PARSER_FILE_PATH << endl;

	std::ifstream file(CONFIG_PARSER_FILE_PATH);
	if (!file)
	{
		//cout << "failed to load file: " << CONFIG_PARSER_FILE_PATH << endl;
		return false;
	}

	try
	{
		std::string line = "";
		std::string key, value;

		while(!file.eof())
		{
			getnextline(file, line);
			const char* cString = line.c_str();
		
			// skip comment lines
			if (cString[0] == '#' || cString[0] == '!')
				continue;

			size_t pos = line.find(" = ");
			if ((int)pos > 0)
			{
				key = line.substr(0, pos);
				value = line.substr(pos+3);
				cache[key] = value;

				//if(_DEBUG)
				//	cout << "parsed pair: " << key << " = " << value << endl;
			}
			else
				std::cout << "failed to parse line: " << line << std::endl;

		}
	} catch (...) {
		file.close();
		return false;
	}
	//cout << "parsing successful!" << endl;
	file.close();
	return true;

}


void PropertiesFileParser::getProperty(std::string name, std::string& value)
{
	value = cache[name];
}

void PropertiesFileParser::getProperty(std::string name, int& value)
{
	sscanf(cache[name].c_str(), "%i", &value);
}

void PropertiesFileParser::getProperty(std::string name, float& value)
{
	sscanf(cache[name].c_str(), "%f", &value);
}

void PropertiesFileParser::getProperty(std::string name, bool& value)
{
	int foo;
	sscanf(cache[name].c_str(), "%i", &foo);
	value = (foo > 0);
}

void PropertiesFileParser::dumpCache()
{
	std::cout << "dumping shader cache ---------------------------" << std::endl;

	for (PropertiesCache::iterator it = cache.begin(); it != cache.end(); it++)
		std::cout << it->first << " = " << it->second << std::endl;

	std::cout << "------------------------------------------------" << std::endl;
}