#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>


void _readfile(
		const char* filename,
		std::vector<unsigned char>& content);

void _writefile(
		const char* filename,
		const std::vector<unsigned char>& content);

void _appendfile(
		const char* filename,
		const std::vector<unsigned char>& content);

void _create_parent_directories(const char* filename);
void _create_directories(const std::string& dir);

int _random(
		const long min,
		const long max);

std::string _gethostname();

#endif
