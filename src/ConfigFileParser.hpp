#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include "common.hpp"


/**
 * parse the config file and provide the app with the config values. Singleton
 */

/** define the path to the config file */
#define CONFIG_PARSER_FILE_PATH "kocmoc.cfg"

class ConfigFileParser
{
public:
	/**
	 * Get the singleton instance
	 */
	static ConfigFileParser& GetInstance(void);

	/**
	 * parse the config file. Make sure to call this method bevore accessing
	 * any public configuration parameters.
	 *
	 * @return true iff parsinf succeeds. 
	 */
	bool parse(void);

	~ConfigFileParser(void);

	// config options, default values are provided in the c'tor
	GLint width;
	GLint height;
	bool fullscreen;
	bool forceCaptureMouse;
	bool debugShowBulletWireframe;
	bool forceInstancingOff;
	bool enableFBOs;
	bool showFrameRate;
	bool muteAudio;
	bool forceTerrainOff;
	bool forceFreeLookCamera;
	bool forceHDRNoMipMap;

private:
		ConfigFileParser(void);
		ConfigFileParser(const ConfigFileParser &cc);

		bool isspacesonly(const string& line);
		void getnextline(std::istream& is, string& line);
};

#endif
