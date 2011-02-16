#ifndef KOCMOC_SYMBOL_HPP_
#define KOCMOC_SYMBOL_HPP_

#include <cstring>


/**
 * This symbol implementation is base on the symbol implementation
 * by Broken Rules GmbH, (c) 2010
 */

namespace kocmoc
{
	typedef const char* Symbol;
	
	Symbol symbolize(const char* string);

	/**
	 * a struct to compare two char*s
	 */

	struct charComparator
	{
		bool operator() (const char* lhs, const char* rhs) const
		{
			return strcmp(lhs, rhs) < 0;
		}
	};
}

#endif