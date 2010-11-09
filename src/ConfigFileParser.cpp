#include "ConfigFileParser.h"
#include <fstream>

ConfigFileParser::ConfigFileParser(void)
{
	//default values
	width = 1280;
	height = 720;
	fullscreen = false;
	forceCaptureMouse = true;
	debugShowBulletWireframe = false;
	forceInstancingOff = false;
	enableFBOs = true;
	showFrameRate = true;
	muteAudio = false;
	forceTerrainOff = false;
	forceFreeLookCamera = false;
	forceHDRNoMipMap = false;
}

ConfigFileParser::~ConfigFileParser(void)
{
}

ConfigFileParser& ConfigFileParser::GetInstance()
{
	static ConfigFileParser instance;
	return instance;
}

bool ConfigFileParser::isspacesonly(const string& line)
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

void ConfigFileParser::getnextline(std::istream& is, string& line) 
{
	while (!is.eof()) {
		getline(is, line);

		if (!isspacesonly(line)) return;
	}
}

bool ConfigFileParser::parse()
{
	cout << "parsing config file: " << CONFIG_PARSER_FILE_PATH << endl;

	std::ifstream file(CONFIG_PARSER_FILE_PATH);
	if (!file)
	{
		cout << "failed to load file: " << CONFIG_PARSER_FILE_PATH << endl;
		return false;
	}

	try
	{
		string line = "";
		int matched = 0;
		int boolInt = 0;

		while(!file.eof())
		{
			getnextline(file, line);
			const char* cString = line.c_str();
		
			if (cString[0] == '#') 
				continue;

			matched = sscanf(cString, "width = %d", &width);
			if (matched > 0) continue;

			matched = sscanf(cString, "height = %d", &height);
			if (matched > 0) continue;

			matched = sscanf(cString, "fullscreen = %d", &boolInt);
			if (matched > 0)
			{
				fullscreen = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "forceCaptureMouse = %d", &boolInt);
			if (matched > 0)
			{
				forceCaptureMouse = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "debugShowBulletWireframe = %d", &boolInt);
			if (matched > 0) 	
			{
				debugShowBulletWireframe = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "forceInstancingOff = %d", &boolInt);
			if (matched > 0) 	
			{
				forceInstancingOff = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "enableFBOs = %d", &boolInt);
			if (matched > 0) 	
			{
				enableFBOs = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "showFrameRate = %d", &boolInt);
			if (matched > 0) 	
			{
				showFrameRate = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "muteAudio = %d", &boolInt);
			if (matched > 0) 	
			{
				muteAudio = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "forceTerrainOff = %d", &boolInt);
			if (matched > 0) 	
			{
				forceTerrainOff = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "forceFreeLookCamera = %d", &boolInt);
			if (matched > 0) 	
			{
				forceFreeLookCamera = (boolInt == 1);
				continue;
			}

			matched = sscanf(cString, "forceHDRNoMipMap = %d", &boolInt);
			if (matched > 0) 	
			{
				forceHDRNoMipMap = (boolInt == 1);
				continue;
			}

			cout << "failed to parse line: " << line << endl;

		}
	} catch (...) {
		return false;
	}
	cout << "parsing successful!" << endl;
	return true;
}
