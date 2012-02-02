#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>


bool _readfile(
		const char* filename,
		std::vector<unsigned char>& content);

void _writefile(
		const char* filename,
		const std::vector<unsigned char>& content);

#endif
