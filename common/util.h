#ifndef __UTIL_H__
#define __UTIL_H__

#include <sys/time.h>

#include <vector>
#include <string>
#include <stdexcept>


void _readfile(
		const char* filename,
		std::vector<unsigned char>& content);

void _readfile(
		const char* filename,
		std::string& content);

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

void _splitfile(const char* filename, int chunksize,int& parts);

class StopWatch
{
public:
	StopWatch()
	{
		if (gettimeofday(&_start, NULL) == -1)
			throw std::runtime_error("gettimeofday");
	}


	unsigned int GetMilli()
	{
		struct timeval cur;

		if (gettimeofday(&cur, NULL) == -1)
			throw std::runtime_error("gettimeofday");

		return static_cast<unsigned int> (
				(cur.tv_sec * 1000.0 + cur.tv_usec / 1000.0)
				- (_start.tv_sec * 1000.0 + _start.tv_usec / 1000.0) );
	}


	unsigned int GetMicro()
	{
		struct timeval cur;

		if (gettimeofday(&cur, NULL) == -1)
			throw std::runtime_error("gettimeofday");

		return static_cast<unsigned int> (
				(cur.tv_sec * 1000000.0 + cur.tv_usec)
				- (_start.tv_sec * 1000000.0 + _start.tv_usec) );
	}


	void Reset()
	{
		if (gettimeofday(&_start, NULL) == -1)
			throw std::runtime_error("gettimeofday");
	}


private:
	struct timeval _start;
};

#endif
