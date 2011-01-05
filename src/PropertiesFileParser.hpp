#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include "common.hpp"
#include <map>

namespace kocmoc
{
	namespace util
	{


		/**
		 * parse the config file and provide the app with the config values. Singleton
		 */

		/** define the path to the config file */
		#define CONFIG_PARSER_FILE_PATH "kocmoc.properties"

		class PropertiesFileParser
		{
		public:
			/**
			 * Get the singleton instance
			 */
			static PropertiesFileParser& GetInstance(void);

			/**
			 * parse the config file. Make sure to call this method bevore accessing
			 * any public configuration parameters.
			 *
			 * @return true iff parsinf succeeds. 
			 */
			bool parse(void);

			~PropertiesFileParser(void);

			/**
			 * Get a property with the given name.
			 */
			void getProperty(string name, string* value);
			void getProperty(string name, int* value);
			void getProperty(string name, float* value);
			void getProperty(string name, bool* value);

			bool getBoolean(std::string name);

			/**
			 * Print the contents of the cache to the std out
			 */
			void list(void);

		private:
				PropertiesFileParser(void);
				PropertiesFileParser(const PropertiesFileParser &cc);

				bool isspacesonly(const string& line);
				void getnextline(std::istream& is, string& line);

				std::map<string, string> cache;
		};
	}
}
#endif
