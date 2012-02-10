#include <stdlib.h>
#include <libgen.h>
#include <linux/limits.h>

#include <fstream>
#include <iostream>

#include <boost/filesystem.hpp>

#include "util.h"

using namespace std;

// TODO: generate flecs exceptions instead of boolean value.
//  FleCS::Error
//  FleCS::Error::Client
//  FleCS::Error::Server
void _readfile(
		const char* filename,
		vector<unsigned char>& content)
{
	ifstream file(filename, ios::in | ios::binary | ios::ate);

	if (! file.is_open())
	{
		cerr << "Unable to open file " << filename << "\n";
		exit(EXIT_FAILURE);
	}

	ifstream::pos_type size;
	size = file.tellg();

	char* memblock;
	memblock = new char [size];

	file.seekg (0, ios::beg);
	file.read (memblock, size);
	file.close();

	content.insert(content.begin(), memblock, memblock + size);

	delete[] memblock;
}


void _writefile(
		const char* filename,
		const vector<unsigned char>& content)
{
	_create_parent_directories(filename);

	ofstream outfile(filename, ios::out | ios::binary);

	if (outfile.fail())
	{
		cerr << "Unable to open file " << filename << "\n";
		exit(EXIT_FAILURE);
	}

	outfile.write(reinterpret_cast<const char*>(&content[0]), content.size());
}


void _appendfile(
		const char* filename,
		const vector<unsigned char>& content)
{
	_create_parent_directories(filename);

	ofstream outfile(filename, ios::out | ios::binary | ios::app);

	if (outfile.fail())
	{
		cerr << "Unable to open file " << filename << "\n";
		exit(EXIT_FAILURE);
	}

	outfile.write(reinterpret_cast<const char*>(&content[0]), content.size());
}


void _create_parent_directories(const char* filename)
{
	char filename_[PATH_MAX];
	strcpy(filename_, filename);
	char* dir = dirname(filename_);

	if ((strcmp(dir, ".") != 0) && (strcmp(dir, "/") != 0))
		_create_directories(dir);
}


void _create_directories(const string& dir)
{
	boost::system::error_code ec;
	bool r = boost::filesystem::create_directories(dir, ec);
	if (!r && ec != boost::system::errc::success)
	{
		cerr << "create_directories. ec=" << ec << "\n";
		exit(EXIT_FAILURE);
	}
}


int _random(
		const long min,
		const long max)
{
	long r = random();

	// ((double) r / (RAND_MAX + 1)) : [0:1)
	
	return (int) ((double) (max - min + 1) * ( (double) r / (RAND_MAX + 1.0))) + min;
}


string _gethostname()
{
	const int LEN = 100;
	char hn[LEN];

	if (gethostname(hn, LEN) != 0)
	{
		perror("gethostname");
		exit(1);
	}

	return hn;
}
