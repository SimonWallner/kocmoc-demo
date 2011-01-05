#ifndef _PROPERTY_HPP_
#define _PROPERTY_HPP_

#include <string>

namespace kocmoc
{
	namespace util
	{
		/**
		 * This class is a convenience wrapper to easily access properties through
		 * the propertiesFileParser, without using its unhandy syntax.
		 * usage example 1: float width = util::Property("windowWidth");
		 * usage example 2: if (util::Property("awesome")) { do awesome stuff }
		 */
		class Property
		{
		public:
			Property(std::string name);
			~Property(void);

			operator bool ();
			operator int ();
			operator float ();
			operator double();
			operator std::string();
			
		private:
			std::string name;
		};
	}
}

#endif