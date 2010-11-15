#include "PropertiesFileParser.hpp"
#include <fstream>

PropertiesFileParser::PropertiesFileParser(void)
{
	//default values
	cout << "pre-populating props cache with default values" << endl;
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

bool PropertiesFileParser::isspacesonly(const string& line)
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

void PropertiesFileParser::getnextline(std::istream& is, string& line) 
{
	while (!is.eof()) {
		getline(is, line);

		if (!isspacesonly(line)) return;
	}
}

bool PropertiesFileParser::parse()
{
	cout << "parsing properties file: " << CONFIG_PARSER_FILE_PATH << endl;
	return false;

	std::ifstream file(CONFIG_PARSER_FILE_PATH);
	if (!file)
	{
		cout << "failed to load file: " << CONFIG_PARSER_FILE_PATH << endl;
		return false;
	}

	try
	{
		string line = "";
		char* key = '\0';
		char* value = '\0';

		while(!file.eof())
		{
			getnextline(file, line);
			const char* cString = line.c_str();
		
			// skip comment lines
			if (cString[0] == '#' || cString[0] == '!')
				continue;

			if (sscanf(cString, "%s", key) > 0 && sscanf(cString, "%*s = %s", value) > 0)
			{
//				cache[key] = value;
				cout << "inserted: " << key << " = " << value << endl;
			}
			else
				cout << "failed to match: " << cString;

		}
	} catch (...) {
		return false;
	}
	cout << "parsing successful!" << endl;
	return true;
}


void PropertiesFileParser::getProperty(string name, string * value)
{
	*value = cache[name];
}

void PropertiesFileParser::getProperty(string name, int *value)
{
	sscanf(cache[name].c_str(), "%i", value);
}

void PropertiesFileParser::getProperty(string name, bool *value)
{
	int foo;
	sscanf(cache[name].c_str(), "%i", &foo);
	*value = (foo > 0);
}
